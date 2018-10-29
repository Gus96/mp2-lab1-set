// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	BitLen = len;
	MemLen = BitLen;
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];//T.elem
	for (int i = 0; i < MemLen; i++)
		this ->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ( n < 0 || n >= BitLen)
		throw n;
	return (n / (sizeof (TELEM) * 8));//ф-я будет вычислять номер
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen)
		throw n;//при n = 32 будет = 0
	int Saw = n % 32;//будем писать единицу на нулевой позиции
	TELEM res = 1;
	res = res << Saw;
	return res;//теперь маска будет давать единицу в нулевой позиции
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return 0;
}

void TBitField::SetBit(const int n) // установить бит
{//данный метод записывает единицу в какую то позицию через логическое "или"
	//проверяет на n диапазон
	int ind = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[ind] |= mask;//как +
}

void TBitField::ClrBit(const int n) // очистить бит
{//ф-я которая очищает бит(записывает нолик) побитовое отрицание это ~b
	int ind = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[ind] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  //return 0;
	int ind = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	if (pMem[ind] & mask == 0)
		return 0;
	else 
		return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{///////////////////////////
	if (this == &bf)
		return *this;
	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  //return 0;
	//сравниваем длины
	if (BitLen != bf.BitLen)
		return 0;
	//сравниваем все ячейки кроме последней
	for (int i = 0; i < MemLen-1; i++)
	{
		if (pMem[i] != bf.pMem[i])
			return 0;
	}
	//побитово сравниваем
	for (int i = (MemLen - 1) * 32; i < BitLen; i++)
	{
		if (GetBit(i) != bf.GetBit(i))
			return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return 0;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{//соединяем элементы
	int len;
	if (BitLen > bf.BitLen)
		len = BitLen;
	else
		len = bf.BitLen;

	TBitField tmp (len);
	for (int i = 0; i < MemLen; i ++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] |= bf.pMem[i];
	return tmp;
}
TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{///////////////////////////////////////////
	int lenght;
	if (BitLen > bf.BitLen)
		lenght = BitLen;
	else
		lenght = bf.BitLen;
	TBitField tmp(lenght);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		tmp.pMem[i] &= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{//////////////
	TBitField tmp(BitLen);
	for (int i = 0; i < BitLen; i++)
		if (this->GetBit(i) == 0)
			tmp.SetBit(i);
		else
			tmp.ClrBit(i);
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
}
