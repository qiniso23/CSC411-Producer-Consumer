#include "consumer.h"
#include <fstream>
#include <iostream>
#include <filesystem>

//***************************************************************************************************************************************************
consumer::consumer(buffer& _buffer)
    : buffer_(_buffer)
{}

//***************************************************************************************************************************************************
void consumer::consume() 
{
    // get file number from buffer
    int file_number = buffer_.consume();

    // create filename and read XML file
    std::string filename = buffer_.get_shared_directory() + "/student" + std::to_string(file_number) + ".xml";
    std::ifstream in_file(filename);

    if (!in_file.is_open()) 
    {
        std::cerr << "[CONSUMER] Error: Could not open file " << filename << "\n";
        return;
    }

    std::string xml_content((std::istreambuf_iterator<char>(in_file)), std::istreambuf_iterator<char>());
    in_file.close();

    std::cout << "[CONSUMER] Read file: " << filename << "\n";

    // parse XML and create ITStudent object
    ITstudent student = ITstudent::from_XML(xml_content);

    // calculate average and determine pass/fail
    print_student_info(student);

    // delete the XML file
    if (std::filesystem::remove(filename))
        std::cout << "[CONSUMER] Deleted file: " << filename << "\n";
    else
        std::cerr << "[CONSUMER] Error: Could not delete file " << filename << "\n";

    // simulate consumption time
    std::this_thread::sleep_for(std::chrono::milliseconds(700));
}

//***************************************************************************************************************************************************
void consumer::run(int _num_students)
{
    std::cout << "[CONSUMER] Starting consumption of " << _num_students << " students...\n";

    for (int i = 0; i < _num_students; i++)
        consume();

    std::cout << "[CONSUMER] Finished consumption.\n";
}

//***************************************************************************************************************************************************
void consumer::print_student_info(const ITstudent& _student) const
{
    std::cout << "\n========================================\n";
    std::cout << "Student Name: " << _student.get_full_name() << "\n";
    std::cout << "Student ID: " << _student.get_student_id() << "\n";
    std::cout << "Programme: " << _student.get_programme() << "\n";
    std::cout << "Courses and Marks:\n";

    for (const auto& course_mark_ : _student.get_course_marks())
        std::cout << "  - " << course_mark_.course_code << ": " << course_mark_.mark << "\n";

    const float avg_mark = _student.calculate_average();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average Mark: " << avg_mark << "\n";
    std::cout << "Status: " << ((avg_mark > 50.0f) ? "PASSED" : "FAILED") << "\n";
    std::cout << "========================================\n";
}