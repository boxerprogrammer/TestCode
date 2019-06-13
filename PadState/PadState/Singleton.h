#pragma once
class Singleton
{
private:
	//�����֎~
	Singleton();
	//�R�s�[�A����֎~
	Singleton(const Singleton&);
	void operator=(const Singleton&);
public:
	Singleton& Instance() {
		static Singleton instance;
		return instance;
	}
	~Singleton();
};

