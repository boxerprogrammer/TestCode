#pragma once
#include<type_traits>
/// <summary>
/// 割り切れるかどうかを返す
/// </summary>
/// <typeparam name="T">整数型に限る</typeparam>
/// <param name="v">チェックしたい値</param>
/// <param name="div">モジュロ値</param>
/// <returns>true:割り切れる</returns>
template<typename T>
inline bool IsDivisible(const T& v, const T& div) {
	static_assert(std::is_integral<T>::value);
	return v % div == 0;
}

/// <summary>
/// divを法とした剰余インクリメント
/// インクリメントの結果が第二引数modulo以上になったときまた0に戻り
/// 永遠に回り続ける
/// </summary>
/// <typeparam name="T">整数型に限る</typeparam>
/// <param name="value">現在の値</param>
/// <param name="modulo">モジュロ値</param>
/// <returns>剰余加算を行った値</returns>
template<typename T>
inline T ModuloIncrement(const T& value, const T& modulo) {
	static_assert(std::is_integral<T>::value);
	return (value + 1) % modulo;
}

extern float Sign(float value);