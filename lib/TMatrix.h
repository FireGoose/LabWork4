#pragma once
#include "TVector.h"
#include <math.h>
#include <fstream>


using namespace std;
template<class T>
class TMatrix : public TVector<TVector<T>>
{
public:
    TMatrix();
    TMatrix(int l, int h);
    TMatrix(const TMatrix& a);
    TMatrix(TMatrix&& a);
    ~TMatrix();

    int GetRows() const;
    int GetColumns() const;

    TMatrix<T> operator+(const TMatrix<T>& a);
    TMatrix<T> operator-(const TMatrix<T>& a);
    TMatrix<T> operator*(const TMatrix<T>& a);
    TMatrix<T> operator*(const T a);
    TMatrix<T> operator/(const T a);

    TMatrix<T>& operator=(const TMatrix<T>& a);
    TMatrix<T>& operator=(TMatrix<T>&& a);
    bool operator==(const TMatrix<T>& a);

    template <class O>
    friend ostream& operator<<(ostream& o, TMatrix<O>& a);
    template <class I>
    friend istream& operator>>(istream& i, TMatrix<I>& a);

    void RandMatrix();
    virtual void Save(const char* path = "./data.txt");
    virtual void Read(const char* path = "./data.txt");

    void PrintSystem(TVector<double> &consts);
    void Gauss(TVector<double> &consts);

    int ElementCount(const T el);
    TMatrix Positions(const T el);
};

template<class T>
inline TMatrix<T>::TMatrix() : TVector<TVector<T>>::TVector() {}

template<class T>
inline TMatrix<T>::TMatrix(int rows, int columns) : TVector<TVector<T>>::TVector(rows)
{
    for (int i = 0; i < rows; ++i)
    {
        (*this)[i] = TVector<T>(columns);
    }
}

template<class T>
TMatrix<T>::TMatrix(const TMatrix<T> &obj) : TVector<TVector<T>>::TVector(obj.GetLen())
{
    int rows = obj.GetLen();
    int columns = obj.GetColumns();
    for (int i = 0; i < rows; i++)
    {
        this->vector[i] = TVector<T>(columns);
        for (int j = 0; j < columns; j++)
        {
            this->vector[i][j] = obj.vector[i][j];
        }
    }
}

template<class T>
TMatrix<T>::TMatrix(TMatrix<T> &&obj)
{
    this->len = obj.len;
    this->vector = obj.vector;
    this->isNew = obj.isNew;
    obj.len = 0;
    obj.vector = nullptr;
    obj.isNew = false;
}

template<class T>
inline TMatrix<T>::~TMatrix() {}

template<class T>
inline int TMatrix<T>::GetRows() const
{
    return (*this).GetLen();
}

template<class T>
inline int TMatrix<T>::GetColumns() const
{
    if (GetRows() == 0) return 0;
    return (*this)[0].GetLen();
}

template<class T>
inline TMatrix<T> TMatrix<T>::operator+(const TMatrix& obj)
{
    int row = GetRows();
    int col = GetColumns();
    if (row != obj.GetRows() || col != obj.GetColumns()) throw("Different size");
    TMatrix<T> res(row, col);
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j) res.vector[i][j] = (*this)[i][j] + obj[i][j];
    return res;
}

template<class T>
inline TMatrix<T> TMatrix<T>::operator-(const TMatrix& obj)
{
    int row = GetRows();
    int col = GetColumns();
    if (row != obj.GetRows() || col != obj.GetColumns()) throw("Different size");
    TMatrix<T> res(row, col);
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j) res.vector[i][j] = (*this)[i][j] - obj[i][j];
    return res;
}

template<class T>
inline TMatrix<T> TMatrix<T>::operator*(const T mul)
{
    int row = GetRows();
    int col = GetColumns();
    TMatrix<T> res(row, col);
    if (mul == 0) return res;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j) res.vector[i][j] = (*this)[i][j] * mul;
    return res;
}

template<class T>
inline TMatrix<T> TMatrix<T>::operator/(const T div)
{
    int row = GetRows();
    int col = GetColumns();
    if (div == 0) throw("Division by zero");
    TMatrix<T> res(row, col);
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j) res.vector[i][j] = (*this)[i][j] / div;
    return res;
}

template<class T>
inline TMatrix<T> TMatrix<T>::operator*(const TMatrix& obj)
{
    int row = GetRows();
    int col = GetColumns();
    if (row != obj.GetColumns()) throw("Sizes don't match");
    TMatrix<T> res(row, col);
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < obj.GetColumns(); ++j)
        {
            res.vector[i][j] = 0;
            for (int k = 0; k < col; ++k) res.vector[i][j] += (*this)[i][k] * obj[k][j];
        }
    return res;
}

template<class T>
inline TMatrix<T>& TMatrix<T>::operator=(const TMatrix<T>& obj)
{
    int row = GetRows();
    int col = GetColumns();
    if (row != obj.GetRows() || col != obj.GetColumns()) throw("Different sizes");
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j) (*this)[i][j] = obj[i][j];
    return *this;
}

template<class T>
inline TMatrix<T>& TMatrix<T>::operator=(TMatrix<T>&& obj)
{
    *this = obj;
    return *this;
}

template<class T>
inline bool TMatrix<T>::operator==(const TMatrix<T>& obj)
{
    int row = GetRows();
    int col = GetColumns();
    if (row != obj.GetRows() || col != obj.GetColumns()) throw("Different sizes");
    bool flag = true;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j) if ((*this)[i][j] != obj[i][j]) flag = false;
    return flag;
}
// Output
template <class O>
inline ostream& operator<<(ostream& o, TMatrix<O>& p)
{
    int row = p.GetRows();
    int col = p.GetColumns();
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            o << p[i][j] << " ";
        }
        o << endl;
    }
    return o;
}
// Input
template <class I>
inline istream& operator>>(istream& is, TMatrix<I>& p)
{
    int row = p.GetRows();
    int col = p.GetColumns();
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            cout <<"a[" << i << "][" << j << "]:  ";
            is >> p[i][j];
            cout << endl;
        }
    }
    return is;
}

template<class T>
inline void TMatrix<T>::RandMatrix()
{
    int s1 = (*this).GetRows();
    int s2 = (*this).GetColumns();
    for (int i = 0; i < s1; ++i)
    {
        for (int j = 0; i < s2; ++j)
            (*this)[i][j] = rand();
    }
}

template<class T>
inline void TMatrix<T>::Save(const char* path)
{
    ofstream FileLoc(path);
    if (!FileLoc.is_open()) throw("File error");
    if (FileLoc.is_open())
    {
        FileLoc << GetRows() << "\n";
        FileLoc << GetColumns() << "\n";
        for (int i = 0; i < GetRows(); ++i)
        {
            for (int j = 0; j < GetColumns(); ++j)
                FileLoc << (*this)[i][j] << "\n";
        }
    }
    FileLoc.close();
}

template<class T>
inline void TMatrix<T>::Read(const char* path)
{
    ifstream FileLoc(path);
    if (!FileLoc.is_open()) throw("File error");
    if (FileLoc.is_open())
    {
        int row = 0, col = 0;
        FileLoc >> row;
        FileLoc >> col;
        if (row != GetRows() || col != GetColumns()) throw("Different sizes");
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < col; ++j)
                FileLoc >> (*this)[i][j];
    }
    FileLoc.close();
}

template <class T>
inline int TMatrix<T>::ElementCount(const T el)
{
    int row = GetRows();
    int col = GetColumns();
    int ans = 0;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j) if ((*this)[i][j] == el) ans++;
    return ans;
}

template <class T>
inline TMatrix<T> TMatrix<T>::Positions(const T el)
{
    int row = GetRows();
    int col = GetColumns();
    int count = ElementCount(el);
    TMatrix<T> ans(count, 2);
    int n = 0;
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            if ((*this)[i][j] == el)
            {
                ans[n][0] = i;
                ans[n++][1] = j;
            }
    return ans;
}

template <class T>
inline void TMatrix<T>::PrintSystem(TVector<double> &consts)
{
    int n = consts.GetLen();
    if (n != GetRows()) throw("Length error");
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cout << (*this)[i][j] << "*x" << j+1;
            if (j < n - 1) cout << " + ";
        }
        cout << " = " << consts[i] << endl;
    }
}

template <class T>
inline void TMatrix<T>::Gauss(TVector<double> &consts)
{
    int n = consts.GetLen();
    TVector<double> x(n);
    double max;
    int index;
    const double accuracy = pow(10,-10);
    double temp;
    int k = 0;
    while (k < n)
    {
        max = abs((*this)[k][k]);
        index = k;
        for (int i = k + 1; i < n; i++) if (abs((*this)[i][k]) > max)
            {
                max = abs((*this)[i][k]);
                index = i;
            }

        if (max < accuracy)
        {
            throw("Zero column");
        }

        for (int j = 0; j < n; j++)
        {
            temp = (*this)[k][j];
            (*this)[k][j] = (*this)[index][j];
            (*this)[index][j] = temp;
        }

        temp = consts[k];
        consts[k] = consts[index];
        consts[index] = temp;

        for (int i = k; i < n; i++)
        {
            temp = (*this)[i][k];
            if (abs(temp) < accuracy)
                continue;
            for (int j = k; j < n; j++) (*this)[i][j] /= temp;
            consts[i] /= temp;
            if (i == k)
                continue;
            for (int j = 0; j < n; j++) (*this)[i][j] -= (*this)[k][j];
            consts[i] -= consts[k];
        }
        k++;
    }

    for (k = n - 1; k > -1; k--)
    {
        x[k] = consts[k];
        for (int i = 0; i < k; i++)
            consts[i] -= (*this)[i][k] * x[k];
    }

    for (k = n - 1; k >= 0; k--)
    {
        for (int i = 0; i < k; i++)
        {
            double factor = (*this)[i][k];
            x[i] -= factor * x[k];
            (*this)[i][k] = 0.0;
        }
    }

    for (int i = 0; i < n; i++) {
        consts[i] = x[i];
    }

    (*this).PrintSystem(consts);
}



