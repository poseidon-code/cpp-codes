#include <source_location>
#include <stacktrace>
#include <string>
#include <iostream>



template <typename DataType>
class Exception {
private:
    std::string error_string;
    DataType user_data;
    const std::source_location location;
    const std::stacktrace backtrace;


public:
    Exception(
        std::string error_string, 
        DataType user_data, 
        const std::source_location& location = std::source_location::current(),
        std::stacktrace backtrace = std::stacktrace::current()
    ) : 
        error_string(std::move(error_string)),
        user_data(std::move(user_data)),
        location(location),
        backtrace(std::move(backtrace)) 
    {};

    std::string& what() { return this->error_string; }
    const std::string& what() const noexcept { return this->error_string; }

    const std::source_location& where() const noexcept { return this->location; }
    
    const std::stacktrace& stack() const noexcept { return this->backtrace; }
    
    DataType& data() { return this->user_data; }
    const DataType& data() const noexcept { return this->user_data; }

    std::ostream& operator<<(std::ostream& os, const std::stacktrace& backtrace) {
        for (auto i = backtrace.begin(); i != (backtrace.end()-3); ++i) {
            od << i->source_file() << "(" << i->source_line() << ") : " << i->description() << "\n";
        }
        return os;
    }
};


template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const std::source_location& location) {
    os  << location.file_name() << "("
        << location.line() << ":"
        << location.column() << ") function `"
        << location.function_name() << "`";
    return os;
}



int main() {
    struct User {
        int id;
        std::string name;
    };

    using UserException = Exception<User>;

    try {
        throw UserException("user already registered", User{1, "poseidon"});
    } catch(UserException& e) {
        std::cerr << "[" << e.where() << "] ";
        std::cerr << "failed to create user with ID (" << e.data().id << ") - " << e.what() << "\n";
        std::cerr << e.stack();
    }
    
    return 0;
}
