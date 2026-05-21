using UnityEngine;
using UnityEditor;
using System.IO;

public class PositionExporter
{

    static void WriteVector3(BinaryWriter bw,Vector3 v)
    {
        bw.Write(v.x);
        bw.Write(v.y);
        bw.Write(v.z);
    }
    static void WriteVector4(BinaryWriter bw, Vector4 v)
    {
        bw.Write(v.x);
        bw.Write(v.y);
        bw.Write(v.z);
        bw.Write(v.w);
    }
    /// <summary>
    /// 内部にメッシュを持っているオブジェクトが何個あるかカウントします
    /// </summary>
    /// <param name="obj">調べたい階層型オブジェクト</param>
    /// <returns>再帰的にたどって見つけたオブジェクトの数</returns>
    static int GetAllObjectsCount(GameObject obj)
    {
        int count = 0;//トータルカウントを記録するためのカウント

        int childCount = obj.transform.childCount;//自分の下にいくつ子があるか
        for (int i = 0; i < childCount; i++)
        {
            var child = obj.transform.GetChild(i).gameObject;
            var mesh = child.GetComponent<MeshFilter>();
            if (mesh != null)
            {
                ++count;
            }
            if (child.transform.childCount > 0)
            {
                count += GetAllObjectsCount(child);
            }
        }
        return count;
    }

    static void RecursiveWriteData(BinaryWriter bw, GameObject obj)
    {
        if (obj == null) return;
        int count = obj.transform.childCount;
        for (int i = 0; i < count; i++)
        {
            var child = obj.transform.GetChild(i);

            var mesh = child.GetComponent<MeshFilter>();
            if (mesh != null)
            {
                bw.Write(mesh.sharedMesh.name);

                //座標の出力
                WriteVector3(bw, child.transform.position);

                Vector3 axis;
                float angle = 0.0f;
                //回転角と回転軸を取得します
                child.rotation.ToAngleAxis(out angle, out axis);
                //回転の出力
                WriteVector3(bw, axis);//回転軸
                bw.Write(angle);//回転角

                //拡大縮小の出力
                WriteVector3(bw, child.localScale);
            }
            //もし、自分の下に子供データがあったらそれも書き込む
            if(child.childCount > 0)
            {
                RecursiveWriteData(bw, child.gameObject);
            }

        }

    }

    [MenuItem("情報出力/配置情報出力")]
    static void OutputLocationData()
    {
        var path = EditorUtility.SaveFilePanel("配置データ出力",
                                    ".",
                                    "location.dat",
                                    "dat");
        if(path == "")
        {
            EditorUtility.DisplayDialog("出力失敗",
                                        "パスがないため中断します",
                                        "閉じる");
            return;
        }

        FileStream fileStream = new FileStream(path, FileMode.Create);
        BinaryWriter bw = new BinaryWriter(fileStream);

        var rootNode = Selection.activeGameObject;//選択しているゲームオブジェクト
        //var count = rootNode.transform.childCount;
        var count = GetAllObjectsCount(rootNode);

        bw.Write(count);

        RecursiveWriteData(bw, rootNode);


        bw.Close();
        fileStream.Close();


        EditorUtility.DisplayDialog("出力完了",
                                        path+"に出力されました",
                                        "閉じる");
    }
}
