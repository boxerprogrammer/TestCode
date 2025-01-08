#include "Cuboid.h"
#include<DxLib.h>

void mgt_lib::Cuboid::CalculateVertices()
{
	vertices_[0]=center_-axis_.front*whd_.z-axis_.right*whd_.x+axis_.up*whd_.y;//手前左上
	vertices_[1]=center_-axis_.front*whd_.z+axis_.right*whd_.x+axis_.up*whd_.y;//手前右上
	vertices_[2]=center_-axis_.front*whd_.z-axis_.right*whd_.x-axis_.up*whd_.y;//手前左下
	vertices_[3]=center_-axis_.front*whd_.z+axis_.right*whd_.x-axis_.up*whd_.y;//手前右下
	vertices_[4]=center_+axis_.front*whd_.z-axis_.right*whd_.x+axis_.up*whd_.y;//奥左上
	vertices_[5]=center_+axis_.front*whd_.z+axis_.right*whd_.x+axis_.up*whd_.y;//奥右上
	vertices_[6]=center_+axis_.front*whd_.z-axis_.right*whd_.x-axis_.up*whd_.y;//奥左下
	vertices_[7]=center_+axis_.front*whd_.z+axis_.right*whd_.x-axis_.up*whd_.y;//奥右下
}

mgt_lib::Cuboid::Cuboid():Cuboid(Position3(),Axis3D(),Size3D())
{
}

mgt_lib::Cuboid::Cuboid(const Position3& pos, const Axis3D& ax, const Size3D& size):
	center_(pos),axis_(ax),whd_(size)
{
	CalculateVertices();

	indices_ = {
		0,1,2,3,0,2,1,3,//手前四角
		4,5,6,7,4,6,5,7,//奥四角
		0,4,1,5,2,6,3,7//残り
	};
	img_ = DxLib::LoadGraph(L"img/white.png");

}

void mgt_lib::Cuboid::Draw(const Material& mat)
{
}

void mgt_lib::Cuboid::WireframeDraw()const
{
	std::array<DxLib::VERTEX3D,8> vertices={};
	for(int i=0;i<vertices.size();++i){
		float t = i >= 4 ? 1.0f : 0.0f;
		vertices[i].pos = mgt_lib::VGetFromVector3(vertices_[i]);
		float u = (float)(i % 2);
		vertices[i].u = u * (1.0f - t) + (1.0f - u) * t;
		vertices[i].v = (float)((i/2) % 2);
		vertices[i].norm = mgt_lib::VGetFromVector3(Vector3(-1,1,-1).Normalized());
		vertices[i].dif = DxLib::GetColorU8(255, 255, 255, 255);
		vertices[i].spc = DxLib::GetColorU8(255, 255, 255, 255);	
	}
	DxLib::DrawPrimitiveIndexed3D(vertices.data(), vertices.size(), indices_.data(), indices_.size(), DX_PRIMTYPE_LINELIST, img_, true);

	std::array<DxLib::VERTEX3D,6> axisverts={};
	for(int i=0;i<axisverts.size();++i){
		axisverts[i].norm = mgt_lib::VGetFromVector3(Vector3(-1,1,-1).Normalized());
		axisverts[i].dif = DxLib::GetColorU8(255, 255, 255, 255);
		axisverts[i].spc = DxLib::GetColorU8(255, 255, 255, 255);	
	}
	axisverts[0].pos = VGetFromVector3(center_);
	axisverts[0].dif = DxLib::GetColorU8(255, 0, 0, 255);
	axisverts[1].pos = VGetFromVector3(center_+axis_.right*10.0f);
	axisverts[1].dif = DxLib::GetColorU8(255, 0, 0, 255);
	axisverts[2].pos = VGetFromVector3(center_);
	axisverts[2].dif = DxLib::GetColorU8(0, 0, 255, 255);
	axisverts[3].pos = VGetFromVector3(center_+axis_.up*10.0f);
	axisverts[3].dif = DxLib::GetColorU8(0, 0, 255, 255);
	axisverts[4].pos = VGetFromVector3(center_);
	axisverts[4].dif = DxLib::GetColorU8(0, 255, 0, 255);
	axisverts[5].pos = VGetFromVector3(center_+axis_.front*10.0f);
	axisverts[5].dif = DxLib::GetColorU8(0, 255, 0, 255);
	DxLib::DrawPrimitive3D(axisverts.data(), axisverts.size(), DX_PRIMTYPE_LINELIST, img_, false);
}

const mgt_lib::Position3& mgt_lib::Cuboid::GetPos() const
{
	return center_;
}

const mgt_lib::Axis3D& mgt_lib::Cuboid::GetAxis() const
{
	return axis_;
}

const mgt_lib::Size3D& mgt_lib::Cuboid::GetSize() const
{
	return whd_;
}

//void mgt_lib::Cuboid::WireframeDraw2()
//{
//	for(int i=0;i<vb_.size();++i){
//		
//		vb_[i].pos = mgt_lib::VGetFromVector3(vertices_[i]);
//		vb_[i].norm = mgt_lib::VGetFromVector3(vertices_[i].Normalized());
//		vb_[i].dif = DxLib::GetColorU8(255, 255, 255, 255);
//		vb_[i].spc = DxLib::GetColorU8(255, 255, 255, 255);
//	}
//	DxLib::DrawLine3D(vb_[0].pos, vb_[1].pos, 0xffffff);
//	DxLib::DrawPrimitive3D(vb_.data(), vb_.size(), DX_PRIMTYPE_POINTLIST, img_, false);
//	DxLib::DrawPrimitiveIndexed3D(vb_.data(), vb_.size(), indices_.data(), indices_.size(), DX_PRIMTYPE_LINELIST, img_, false);
//}
