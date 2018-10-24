#include <gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);//тинициализация теста
    return RUN_ALL_TESTS();//запуск
}
