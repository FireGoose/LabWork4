#include <gtest.h>
#include "TVector.h"
#include "TMatrix.h"

TEST(TMatrixTest, DefaultConstructor) {
    TMatrix<int> a;
    EXPECT_EQ(a.GetRows(), 0);
    EXPECT_EQ(a.GetColumns(), 0);
}

TEST(TMatrixTest, SquareConstructor) {
    TMatrix<int> a(5,7);
    EXPECT_EQ(a.GetRows(), 5);
    EXPECT_EQ(a.GetColumns(), 7);
}

TEST(TMatrixTest, RowsColumnsConstructor) {
    TMatrix<int> a(3, 4);
    EXPECT_EQ(a.GetRows(), 3);
    EXPECT_EQ(a.GetColumns(), 4);
}

TEST(TMatrixTest, CopyConstructor) {
    TMatrix<int> original(2, 2);
    original[0][0] = 5; original[0][1] = 7;
    original[1][0] = 3; original[1][1] = 4;
    TMatrix<int> copy(original);

    EXPECT_EQ(copy.GetRows(), 2);
    EXPECT_EQ(copy.GetColumns(), 2);
    EXPECT_EQ(copy[0][0], 5);
    EXPECT_EQ(copy[0][1], 7);
    EXPECT_EQ(copy[1][0], 3);
    EXPECT_EQ(copy[1][1], 4);
}

TEST(TMatrixTest, MoveConstructor) {
    TMatrix<int> original(2, 2);
    original[0][0] = 5; original[0][1] = 7;
    original[1][0] = 3; original[1][1] = 4;
    TMatrix<int> moved(std::move(original));

    EXPECT_EQ(moved.GetRows(), 2);
    EXPECT_EQ(moved.GetColumns(), 2);
    EXPECT_EQ(moved[0][0], 5);
    EXPECT_EQ(moved[0][1], 7);
    EXPECT_EQ(moved[1][0], 3);
    EXPECT_EQ(moved[1][1], 4);
    EXPECT_EQ(original.GetRows(), 0);
    EXPECT_EQ(original.GetColumns(), 0);
}

TEST(TMatrixTest, Addition) {
    TMatrix<int> a(2, 2), b(2, 2);
    a[0][0] = 1; a[0][1] = 2; a[1][0] = 3; a[1][1] = 4;
    b[0][0] = 5; b[0][1] = 6; b[1][0] = 7; b[1][1] = 8;
    TMatrix<int> result = a + b;

    EXPECT_EQ(result[0][0], 6);
    EXPECT_EQ(result[0][1], 8);
    EXPECT_EQ(result[1][0], 10);
    EXPECT_EQ(result[1][1], 12);
}

TEST(TMatrixTest, Subtraction) {
    TMatrix<int> a(2, 2), b(2, 2);
    a[0][0] = 1; a[0][1] = 2; a[1][0] = 3; a[1][1] = 4;
    b[0][0] = 5; b[0][1] = 6; b[1][0] = 7; b[1][1] = 8;
    TMatrix<int> result = a - b;

    EXPECT_EQ(result[0][0], -4);
    EXPECT_EQ(result[0][1], -4);
    EXPECT_EQ(result[1][0], -4);
    EXPECT_EQ(result[1][1], -4);
}

TEST(TMatrixTest, MatrixMultiplication) {
    TMatrix<int> a(2, 3), b(3, 2);
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 4; a[1][1] = 5; a[1][2] = 6;
    b[0][0] = 7; b[0][1] = 8;
    b[1][0] = 9; b[1][1] = 10;
    b[2][0] = 11; b[2][1] = 12;
    TMatrix<int> result = a * b;

    EXPECT_EQ(result[0][0], 58);
    EXPECT_EQ(result[0][1], 64);
    EXPECT_EQ(result[1][0], 139);
    EXPECT_EQ(result[1][1], 154);
}

TEST(TMatrixTest, ScalarMultiplication) {
    TMatrix<int> a(2, 2);
    a[0][0] = 5; a[0][1] = 7; a[1][0] = 3; a[1][1] = 4;
    TMatrix<int> result = a * 2;

    EXPECT_EQ(result[0][0], 10);
    EXPECT_EQ(result[0][1], 14);
    EXPECT_EQ(result[1][0], 6);
    EXPECT_EQ(result[1][1], 8);
}

TEST(TMatrixTest, DivisionByScalar) {
    TMatrix<int> a(2, 2);
    a[0][0] = 6; a[0][1] = 12; a[1][0] = 18; a[1][1] = 24;
    TMatrix<int> result = a / 6;

    EXPECT_EQ(result[0][0], 1);
    EXPECT_EQ(result[0][1], 2);
    EXPECT_EQ(result[1][0], 3);
    EXPECT_EQ(result[1][1], 4);
}

TEST(TMatrixTest, CopyAssignment) {
    TMatrix<int> a(2, 2), b(2,2);
    a[0][0] = 5; a[0][1] = 7; a[1][0] = 3; a[1][1] = 4;
    b = a;

    EXPECT_EQ(b[0][0], 5);
    EXPECT_EQ(b[0][1], 7);
    EXPECT_EQ(b[1][0], 3);
    EXPECT_EQ(b[1][1], 4);
}

TEST(TMatrixTest, EqualityOperator) {
    TMatrix<int> a(2, 2), b(2, 2);
    a[0][0] = 1; a[0][1] = 2; a[1][0] = 3; a[1][1] = 4;
    b[0][0] = 1; b[0][1] = 2; b[1][0] = 3; b[1][1] = 4;

    EXPECT_TRUE(a == b);
    b[0][0] = 5;
    EXPECT_FALSE(a == b);
}

TEST(TMatrixFileIOTest, SaveAndReadFile) {
    TMatrix<int> original(2, 3);
    original[0][0] = 1; original[0][1] = 2; original[0][2] = 3;
    original[1][0] = 4; original[1][1] = 5; original[1][2] = 6;

    const char* testFileName = "test1.txt";

    original.Save(testFileName);
    
    std::ifstream testFile(testFileName);
    EXPECT_TRUE(testFile.is_open());
    testFile.close();
    
    TMatrix<int> loaded(2, 3);
    loaded.Read(testFileName);
    
    EXPECT_EQ(2, loaded.GetRows());
    EXPECT_EQ(3, loaded.GetColumns());

    for (int i = 0; i < original.GetRows(); ++i) {
        for (int j = 0; j < original.GetColumns(); ++j) {
            EXPECT_EQ(original[i][j], loaded[i][j]);
        }
    }
    
    remove(testFileName);
}

TEST(TMatrixFileIOTest, ReadWithWrongSize) {
    TMatrix<int> originalMatrix(2, 2);
    originalMatrix[0][0] = 1; originalMatrix[0][1] = 2;
    originalMatrix[1][0] = 3; originalMatrix[1][1] = 4;

    const char* testFileName = "abc.txt";
    
    originalMatrix.Save(testFileName);
    
    TMatrix<int> wrongSizeMatrix(3, 3);
    EXPECT_THROW(wrongSizeMatrix.Read(testFileName), const char*);
    
    remove(testFileName);
}

TEST(TMatrixFileIOTest, EmptyMatrix) {
    TMatrix<int> empty;
    const char* testFileName = "empty.txt";
    
    empty.Save(testFileName);

    std::ifstream testFile(testFileName);
    int rows, cols;
    testFile >> rows >> cols;
    EXPECT_EQ(0, rows);
    EXPECT_EQ(0, cols);
    testFile.close();

    TMatrix<int> loaded;
    loaded.Read(testFileName);
    EXPECT_EQ(0, loaded.GetRows());
    EXPECT_EQ(0, loaded.GetColumns());

    remove(testFileName);
}

TEST(TMatrixFileIOTest, NonExistentFile) {
    TMatrix<int> a(2, 2);
    const char* nonExistentFile = "zxc.txt";

    EXPECT_THROW(a.Read(nonExistentFile), const char*);
}

TEST(TMatrixTest, ValueCountMethod)
{
    TMatrix<int> a(3, 3);
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 2; a[1][1] = 3; a[1][2] = 2;
    a[2][0] = 3; a[2][1] = 2; a[2][2] = 1;

    EXPECT_EQ(2, a.ElementCount(1));
    EXPECT_EQ(4, a.ElementCount(2));
    EXPECT_EQ(0, a.ElementCount(5));
}

TEST(TMatrixTest, PositionsMethod)
{
    TMatrix<int> a(3, 3);
    a[0][0] = 1; a[0][1] = 2; a[0][2] = 3;
    a[1][0] = 2; a[1][1] = 3; a[1][2] = 2;
    a[2][0] = 3; a[2][1] = 2; a[2][2] = 1;

    TMatrix<int> result = a.Positions(2);
    EXPECT_EQ(4, result.GetRows());
    EXPECT_EQ(2, result.GetColumns());

    bool f1 = false, f2 = false, f3 = false, f4 = false;
    for (int i = 0; i < result.GetRows(); ++i)
    {
        if (result[i][0] == 0 && result[i][1] == 1) f1 = true;
        if (result[i][0] == 1 && result[i][1] == 0) f2 = true;
        if (result[i][0] == 1 && result[i][1] == 2) f3 = true;
        if (result[i][0] == 2 && result[i][1] == 1) f4 = true;
    }
    EXPECT_TRUE(f1);
    EXPECT_TRUE(f2);
    EXPECT_TRUE(f3);
    EXPECT_TRUE(f4);
}

TEST(TMatrixTest, GaussSolving)
{
    TMatrix<double> a(2, 2);
    a[0][0] = 1; a[0][1] = 2;
    a[1][0] = 3; a[1][1] = 4;
    TVector<double> consts(2);
    consts[0] = 3; consts[1] = 7;
    TVector<double> exp(2);
    exp[0] = 1; exp[1] = 1;
    TVector<double> ans = a.Gauss(consts);
    EXPECT_NEAR(ans[0], 1, 1e-4);
    EXPECT_NEAR(ans[1], 1, 1e-4);
}

TEST(TVectorTest, DefaultConstructor) {
    TVector<int> v;
    EXPECT_EQ(v.GetLen(), 0);
}

TEST(TVectorTest, ParamConstructor) {
    TVector<int> v(5);
    EXPECT_EQ(v.GetLen(), 5);
}

TEST(TVectorTest, CopyConstructor) {
    TVector<int> original(3);
    original[0] = 1; original[1] = 2; original[2] = 3;
    TVector<int> copy(original);

    ASSERT_EQ(copy.GetLen(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
}

TEST(TVectorTest, MoveConstructor) {
    TVector<int> original(3);
    original[0] = 1; original[1] = 2; original[2] = 3;
    TVector<int> moved(std::move(original));

    EXPECT_EQ(moved.GetLen(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
    EXPECT_EQ(original.GetLen(), 0);
}

TEST(TVectorTest, AdditionOperator) {
    TVector<int> a(2);
    a[0] = 1; a[1] = 2;
    TVector<int> b(2);
    b[0] = 3; b[1] = 4;
    TVector<int> c = a + b;

    ASSERT_EQ(c.GetLen(), 2);
    EXPECT_EQ(c[0], 4);
    EXPECT_EQ(c[1], 6);
}

TEST(TVectorTest, SubtractionOperator) {
    TVector<int> a(2);
    a[0] = 5; a[1] = 7;
    TVector<int> b(2);
    b[0] = 3; b[1] = 2;
    TVector<int> c = a - b;

    ASSERT_EQ(c.GetLen(), 2);
    EXPECT_EQ(c[0], 2);
    EXPECT_EQ(c[1], 5);
}

TEST(TVectorTest, DotProductOperator) {
    TVector<int> a(3);
    a[0] = 1; a[1] = 2; a[2] = 3;
    TVector<int> b(3);
    b[0] = 4; b[1] = 5; b[2] = 6;
    int dot = a * b;

    EXPECT_EQ(dot, 32); // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
}

TEST(TVectorTest, ScalarMultiplication) {
    TVector<int> a(3);
    a[0] = 1; a[1] = 2; a[2] = 3;
    TVector<int> result = a * 2;

    ASSERT_EQ(result.GetLen(), 3);
    EXPECT_EQ(result[0], 2);
    EXPECT_EQ(result[1], 4);
    EXPECT_EQ(result[2], 6);
}

TEST(TVectorTest, AssignmentOperator) {
    TVector<int> a(2);
    a[0] = 1; a[1] = 2;
    TVector<int> b;
    b = a;

    ASSERT_EQ(b.GetLen(), 2);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
}

TEST(TVectorTest, MoveAssignmentOperator) {
    TVector<int> a(2);
    a[0] = 1; a[1] = 2;
    TVector<int> b;
    b = std::move(a);

    EXPECT_EQ(b.GetLen(), 2);
    EXPECT_EQ(b[0], 1);
    EXPECT_EQ(b[1], 2);
    EXPECT_EQ(a.GetLen(), 0);
}

TEST(TVectorTest, EqualityOperator) {
    TVector<int> a(2);
    a[0] = 1; a[1] = 2;
    TVector<int> b(2);
    b[0] = 1; b[1] = 2;
    TVector<int> c(2);
    c[0] = 3; c[1] = 4;

    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(TVectorTest, IndexOperator) {
    TVector<int> v(3);
    v[0] = 1; v[1] = 2; v[2] = 3;

    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    v[1] = 5;
    EXPECT_EQ(v[1], 5);
}

TEST(TVectorTest, BubbleSort) {
    TVector<int> v(5);
    v[0] = 5; v[1] = 3; v[2] = 4; v[3] = 1; v[4] = 2;
    v.BubbleSort();

    for (int i = 0; i < 4; ++i) {
        EXPECT_LE(v[i], v[i+1]);
    }
}

TEST(TVectorTest, QuickSort) {
    TVector<int> v(5);
    v[0] = 5; v[1] = 3; v[2] = 4; v[3] = 1; v[4] = 2;
    v.QuickSort();

    for (int i = 0; i < 4; ++i) {
        EXPECT_LE(v[i], v[i+1]);
    }
}

TEST(TVectorTest, InsertionSort) {
    TVector<int> v(5);
    v[0] = 5; v[1] = 3; v[2] = 4; v[3] = 1; v[4] = 2;
    v.InsertionSort();

    for (int i = 0; i < 4; ++i)
    {
        EXPECT_LE(v[i], v[i+1]);
    }
}

TEST(TVectorTest, FirstNorm) {
    TVector<double> v(3);
    v[0] = 1.0; v[1] = -2.0; v[2] = -3.0;
    EXPECT_DOUBLE_EQ(v.FirstNorm(), 6.0);
}

TEST(TVectorTest, SecondNorm) {
    TVector<double> v(2);
    v[0] = 3.0; v[1] = 4.0;
    EXPECT_DOUBLE_EQ(v.SecondNorm(), 5.0);
}

TEST(TVectorTest, InfinityNorm) {
    TVector<double> v(4);
    v[0] = 5.0; v[1] = -7.0; v[2] = 3.0; v[3] = 4.0;
    EXPECT_DOUBLE_EQ(v.InfinityNorm(), 7.0);
}

TEST(TVectorTest, HolderNorm) {
    TVector<double> v(2);
    v[0] = 7.0; v[1] = 24.0;
    EXPECT_NEAR(v.HolderNorm(2.0), 25.0, 1e-6);
}
