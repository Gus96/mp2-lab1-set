// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
//множества переводит в битовую строку
TBitField::TBitField(int len)
{
	if (len < 0)
		throw len;
	BitLen = len;
	if (BitLen % 32 == 0)
		MemLen = BitLen / 32;
	else 
		MemLen = BitLen / 32 + 1;//в большую сторону
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;//если побитово, то 64 нуля (для 33х)

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];//T.elem чтобы не писать везде ungigned
	for (int i = 0; i < MemLen; i++)
		this ->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete [] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{//в какой ячейке (интовых) находится число
	if ( n < 0 || n >= BitLen)
		throw n;
	return (n / (sizeof (TELEM) * 8));//telem байты переходим в bit
	//ф-я будет вычислять номер
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{//ставит единичку на нужную позицию n - позиция на котор нужно сдвинуть
	if (n < 0 || n >= BitLen)
		throw n;//при n = 32 будет = 0
	int Saw = n % 32;//на сколько позиций влево сдвинуть единичку
	TELEM res = 1;//31 ноль и на 32 единичка
	res = res << Saw;
	return res;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{//данный метод записывает единицу в какую то позицию через логическое "или"
	//допустим на 13 позицию поставить единичку
	//проверяет на n диапазон
	int ind = GetMemIndex(n);//в первом или во втором int
	TELEM mask = GetMemMask(n);//созд матр где  на n позиции единичка
	pMem[ind] |= mask;//складывается маска и выбранный бит, на нужн позиции единичка
}

void TBitField::ClrBit(const int n) // очистить бит
{//ф-я которая очищает бит(записывает нолик) побитовое отрицание это ~b
	int ind = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[ind] &= ~mask;//~ это инвертирование & умножение
}

int TBitField::GetBit(const int n) const // получить значение бита
{//что лежит на n позиции
	int ind = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	if ((pMem[ind] & mask) == 0)
		return 0;
	else 
		return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;
	delete[] pMem;//удаляем массив битов
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
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
	return !(*this == bf);//переходит в функцию выше и отрицает значение
}
//~побитовое отриц
//! отриц
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
{
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
{
	TBitField tmp(BitLen);
	for (int i = 0; i < BitLen; i++)
		if (this->GetBit(i) == 0)//если на i ноль
			tmp.SetBit(i); //то ставим 1
		else
			tmp.ClrBit(i);//иначе стираем
	return tmp;
}

// ввод/вывод
istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char symbol;
	for (int i = 0; i < bf.GetLength(); i++)
	{
		istr >> symbol;
		if (symbol == '1')
			bf.GetBit(i);
		else if (symbol == '0')
			bf.ClrBit(i);
		else
		{
			for (i; i < bf.GetLength(); i++)
				bf.GetBit(i);
			break;
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i) == 0)
			ostr << 0;
		else
			ostr << 1;
	}
	return ostr;
}
