#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>


using namespace std; // Директива для использования пространства имен std

// Модуль 1: Интерфейс для source
class Source {
public:
    // Читает байт из источника и возвращает его в параметре byte
    virtual bool ReadByte(uint8_t& byte) = 0;
};

// Модуль 2: Интерфейс для sink
class Sink {
public:
    // Записывает строку в приемник
    virtual void WriteString(const string& str) = 0;
};

class ByteConverter {
public:
    // Модуль 3: Конструктор класса ByteConverter
    ByteConverter(Source* source, Sink* sink) : source_(source), sink_(sink), running_(false) {}

    // Запустить конвертер
    void Start() {
        if (!running_) {
            running_ = true;
            thread_ = thread(&ByteConverter::ConvertBytes, this);
        }
    }

    // Остановить конвертер
    void Stop() {
        if (running_) {
            running_ = false;
            cv_.notify_one();
            thread_.join();
        }
    }

private:
    // Модуль 4: Метод для конвертации байтов
    void ConvertBytes() {
        while (running_) {
            uint8_t byte;
            if (source_->ReadByte(byte)) {
                string converted_str = ConvertByte(byte);
                sink_->WriteString(converted_str);
            }
            else {
                // Если чтение завершено, останавливаем поток
                running_ = false;
            }
        }
    }

    // Модуль 5: Метод для конвертации одного байта
    string ConvertByte(uint8_t byte) {
        uint8_t type = (byte >> 6) & 0x03;
        uint8_t data = byte & 0x3F;

        if (type == 0b00) {
            // Беззнаковое целое
            return to_string(data);
        }
        else if (type == 0b01) {
            // Знаковое целое
            int8_t signed_data = static_cast<int8_t>(data);
            return to_string(signed_data);
        }
        else if (type == 0b10) {
            // Буква латинского алфавита
            char letter = 'a' + data;
            return string(1, letter);
        }
        else {
            // Неизвестный тип
            return "";
        }
    }

    Source* source_;
    Sink* sink_;
    thread thread_;
    bool running_;
    mutex mutex_;
    condition_variable cv_;
};

// Модуль 6: Пример реализации source
class ExampleSource : public Source {
public:
    ExampleSource(const vector<uint8_t>& data) : data_(data), index_(0) {}

    // Реализация метода чтения байта из источника
    bool ReadByte(uint8_t& byte) override {
        if (index_ < data_.size()) {
            byte = data_[index_++];
            return true;
        }
        return false;
    }

private:
    const vector<uint8_t>& data_;
    size_t index_;
};

// Модуль 7: Пример реализации sink
class ExampleSink : public Sink {
public:
    // Реализация метода записи строки в приемник
    void WriteString(const string& str) override {
        cout << "Received: " << str << endl;
    }
};

int main() {
    vector<uint8_t> data = { 0b00001001, 0b01000110, 0b11011011 };

    ExampleSource source(data);
    ExampleSink sink;
    ByteConverter converter(&source, &sink);

    converter.Start();
    this_thread::sleep_for(chrono::milliseconds(1000)); //  ожидание для демонстрации работы
    converter.Stop();

    return 0;
}
