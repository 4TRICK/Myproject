#include <iostream>
#include <string>
#include <thread>
#include <mutex>


using namespace std;

// Интерфейс для источника данных
class Source {
public:
    virtual char readByte() = 0;
};

// Интерфейс для приемника данных
class Sink {
public:
    virtual void writeString(const string& str) = 0;
};

// Пример реализации источника данных для ввода с консоли
class ConsoleSource : public Source {
public:
    char readByte() override {
        char byte;
        cin >> byte;
        return byte;
    }
};

// Пример реализации приемника данных для вывода на консоль
class ConsoleSink : public Sink {
public:
    void writeString(const string& str) override {
        cout << "Received: " << str << endl;
    }
};

// Класс, который выполняет чтение, конвертацию и запись данных
class DataProcessor {
public:
    DataProcessor(Source& source, Sink& sink) : source_(source), sink_(sink), running_(false) {}

    // Метод для запуска обработки данных в отдельном потоке
    void start() {
        running_ = true;
        thread_ = thread(&DataProcessor::process, this);
    }

    // Метод для остановки потока
    void stop() {
        running_ = false;
        if (thread_.joinable()) {
            thread_.join();
        }
    }

private:
    Source& source_;
    Sink& sink_;
    thread thread_;
    bool running_;
    mutex mtx_;

    void process() {
        while (running_) {
            char byte = source_.readByte();
            char type = (byte >> 6) & 0x03;
            char data = byte & 0x3F;

            string result;

            if (type == 0b00) {
                // data - беззнаковое целое
                result = to_string(static_cast<unsigned int>(data));
            }
            else if (type == 0b01) {
                // data - знаковое целое
                result = to_string(static_cast<int>(data));
            }
            else if (type == 0b10) {
                // data - буква латинского алфавита
                result = string(1, 'a' + data);
            }
            else {
                // Обработка других значений type по необходимости
            }

            {
                lock_guard<mutex> lock(mtx_);
                sink_.writeString(result);
            }
        }
    }
};

int main() {
    ConsoleSource source;
    ConsoleSink sink;

    DataProcessor processor(source, sink);

    processor.start();

    cout << "Data processing is running. Press 'q' to stop..." << endl;
    while (true) {
        char input;
        cin >> input;
        if (input == 'q' || input == 'Q') {
            break;
        }
    }

    processor.stop();

    return 0;
}
