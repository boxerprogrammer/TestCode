using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class MenuTest
{
    enum ObjectType
    {
        Mesh,
        Camera,
        Light,
    }
    static private void WriteVector3(BinaryWriter bw, Vector3 vec)
    {
        bw.Write(vec.x);
        bw.Write(vec.y);    
        bw.Write(vec.z);    
    }
    [MenuItem("テスト/データ出力")]
    static public void Test()
    {
        var rootGameObject = Selection.activeGameObject;
        if (rootGameObject)
        {
            var path = EditorUtility.SaveFilePanel("配置データ出力", ".", "Data.pos", "pos");
            if (path == "")
            {
                return;
            }
            var stream = File.Create(path);
            var bw = new BinaryWriter(stream);
            char[] signature = new char[4];
            signature = "POS_".ToCharArray();
            float version = 0.01f;
            bw.Write(signature);
            bw.Write(version);
           
            var childNum=rootGameObject.transform.childCount;
            bw.Write((int)childNum);
            for(int i = 0; i < childNum; i++)
            {
                var child=rootGameObject.transform.GetChild(i);

                var camera = child.GetComponent<Camera>();
                var light = child.GetComponent<Light>();

                ObjectType type=ObjectType.Mesh;
                if (camera != null)
                {
                    type = ObjectType.Camera;
                }else if(light != null)
                {
                    type=ObjectType.Light;
                }

                var name = child.name;
               
                byte nameLen=(byte) name.Length;
                bw.Write((byte)type);
                bw.Write(nameLen);
                bw.Write(name.ToCharArray());
                WriteVector3(bw, child.position);
                WriteVector3(bw, child.rotation.eulerAngles);
                if(camera != null)
                {
                    bw.Write(camera.fieldOfView);//画角
                    bw.Write(camera.nearClipPlane);//ニア
                    bw.Write(camera.farClipPlane);//ファー
                }
            }

            stream.Close();
            EditorUtility.DisplayDialog("配置データが出力されました", "データ出力成功", "閉じる");
        }
        else
        {
            EditorUtility.DisplayDialog("ルートがありません", "エラー", "閉じる");
        }
            
    }
}
