// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microso
//ft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>

TBitField::TBitField(int len)
{
	MemLen = (len - 1) / sizeof(int) + 1;
	pMem = new TELEM[MemLen];
	BitLen = len;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	if (pMem != nullptr)
	{
		delete[] pMem;
		pMem = nullptr;
	}

}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n > BitLen)
		throw(exception()); //вызов исключения 
	else
		return n >> 5; //остаток от деления на 32 
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n > BitLen)
		throw(exception());
	else
		return 1 <<(n & 31); //сдвигает один элемент влево на остаток деления на 32 
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n >= 0) || (n <= BitLen))
		pMem[GetMemIndex(n)] |= GetMemMask(n);
	else
		throw(exception());
}

void TBitField::ClrBit(const int n) // очистить бит
{
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;
	for (int i = 0; i < MemLen; i++)
		if (*(pMem+i) != *(bf.pMem+i))
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (BitLen == bf.BitLen)
		return 0;
	for (int i = 0; i < MemLen; i++)
		if (pMem[i] == bf.pMem[i])
			return 0;
	return 1;

}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{

	int len;
	if (bf.BitLen >= BitLen)
		len = bf.BitLen;
	else 
		len = BitLen;
 
	TBitField tmp(len);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] |= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int len;
	if (bf.BitLen >= BitLen)
		len = bf.BitLen;
	else
		len = BitLen;


	TBitField tmp(len);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] &= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{

	TBitField tmp(BitLen);
	for (int i = 0; i < tmp.MemLen - 1; i++)
		 tmp.pMem[i] = ~pMem[i];
	for (int i = tmp.MemLen-1; i < tmp.BitLen; i++)
	{
		if (GetBit(i))
			tmp.ClrBit(i);
		else
			tmp.SetBit(i);
	}
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string tmp;
	istr >> tmp;
	if (tmp.size() > bf.GetLength())
		throw "Incorrect";
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (tmp[i] == '0')
			bf.ClrBit(i);
		else if (tmp[i] == '1')
			bf.SetBit(i);
		else
			throw "Incorrect";
	}
	return istr;
}


ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i))
			ostr << i << " ";
	}
	return ostr;
}
