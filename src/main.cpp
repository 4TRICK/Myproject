#include <iostream>
#include <string>
#include <thread>
#include <mutex>


using namespace std;

// ��������� ��� ��������� ������
class Source {
public:
    virtual char readByte() = 0;
};

// ��������� ��� ��������� ������
class Sink {
public:
    virtual void writeString(const string& str) = 0;
};

// ������ ���������� ��������� ������ ��� ����� � �������
class ConsoleSource : public Source {
public:
    char readByte() override {
        char byte;
        cin >> byte;
        return byte;
    }
};

// ������ ���������� ��������� ������ ��� ������ �� �������
class ConsoleSink : public Sink {
public:
    void writeString(const string& str) override {
        cout << "Received: " << str << endl;
    }
};

// �����, ������� ��������� ������, ����������� � ������ ������
class DataProcessor {
public:
    DataProcessor(Source& source, Sink& sink) : source_(source), sink_(sink), running_(false) {}

    // ����� ��� ������� ��������� ������ � ��������� ������
    void start() {
        running_ = true;
        thread_ = thread(&DataProcessor::process, this);
    }

    // ����� ��� ��������� ������
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
                // data - ����������� �����
                result = to_string(static_cast<unsigned int>(data));
            }
            else if (type == 0b01) {
                // data - �������� �����
                result = to_string(static_cast<int>(data));
            }
            else if (type == 0b10) {
                // data - ����� ���������� ��������
                result = string(1, 'a' + data);
            }
            else {
                // ��������� ������ �������� type �� �������������
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
