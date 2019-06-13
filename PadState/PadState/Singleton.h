#pragma once
class Singleton
{
private:
	//生成禁止
	Singleton();
	//コピー、代入禁止
	Singleton(const Singleton&);
	void operator=(const Singleton&);
public:
	Singleton& Instance() {
		static Singleton instance;
		return instance;
	}
	~Singleton();
};

