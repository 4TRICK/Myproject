#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "test.h" // Включите заголовочный файл для класса DataProcessor

using namespace testing;

// Создайте фейковые (мок) классы для Source и Sink
class MockSource : public Source {
public:
    MOCK_METHOD(char, readByte, (), (override));
};

class MockSink : public Sink {
public:
    MOCK_METHOD(void, writeString, (const std::string& str), (override));
};

// Тестирование класса DataProcessor
TEST(DataProcessorTest, ProcessData) {
    MockSource mockSource;
    MockSink mockSink;

    DataProcessor processor(mockSource, mockSink);

    // Установите ожидаемое поведение мок-классов
    // Мок Source будет возвращать заданные байты для чтения
    EXPECT_CALL(mockSource, readByte())
        .WillOnce(Return(0b00000001)) // Пример данных для обработки
        .WillOnce(Return(0b01000010)); // Пример данных для обработки

    // Мок Sink будет ожидать запись определенных строк
    EXPECT_CALL(mockSink, writeString("1")); // Ожидаемый результат для первого набора данных
    EXPECT_CALL(mockSink, writeString("-2")); // Ожидаемый результат для второго набора данных

    processor.start();

    // Эмулируйте чтение данных и отправку их в DataProcessor
    ON_CALL(mockSource, readByte())
        .WillByDefault(Return(0b00000001))
        .WillByDefault(Return(0b01000010));

    // Эмулируйте отправку данных в DataProcessor
    char data1 = mockSource.readByte();
    char data2 = mockSource.readByte();
    
    // Вызовите метод processData с эмулированными данными
    processor.processData(data1);
    processor.processData(data2);

    processor.stop();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

