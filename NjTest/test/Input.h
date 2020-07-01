#pragma once
class Input
{
	
public:
	Input();
	/// <summary>
	/// ���͓��e���X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �����Ă邩�ǂ�����Ԃ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	/// <returns>������Ă�</returns>
	bool IsPressed(const char* cmd)const;

	/// <summary>
	/// �����ꂽ�u�ԂȂ̂��ǂ�����Ԃ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	/// <returns>
	/// ��������ĂȂ�:false
	/// ��������Ē��O��������Ă�:false
	/// ��������āA���O�͉�����ĂȂ�:true
	///</returns>
	bool IsTriggered(const char* cmd)const;

	/// <summary>
	/// �����ꂽ�u�ԂȂ̂��ǂ�����Ԃ�
	/// </summary>
	/// <param name="cmd">�R�}���h������</param>
	/// <returns>
	/// ��������Ă�:false
	/// ��������ĂȂ����O��������Ȃ�:false
	/// ���O������āA��������ĂȂ�:true
	///</returns>
	bool IsReleased(const char* cmd)const;

};

