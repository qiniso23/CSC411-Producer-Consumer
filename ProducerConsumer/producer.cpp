#include "producer.h"
#include <iostream>
#include <fstream>

//***************************************************************************************************************************************************
producer::producer(buffer& _buffer)
    : buffer_(_buffer)
{
    std::random_device rd;

    generator.seed(rd());
}

//***************************************************************************************************************************************************
std::string producer::generate_random_first_name()
{
    std::uniform_int_distribution<int> dist(0, (int)first_names.size() - 1);

    return first_names[dist(generator)];
}

//***************************************************************************************************************************************************
std::string producer::generate_random_last_name()
{
    std::uniform_int_distribution<int> dist(0, (int)last_names.size() - 1);

    return last_names[dist(generator)];
}

//***************************************************************************************************************************************************
int producer::generate_random_id()
{
    std::uniform_int_distribution<int> dist(10000000, 99999999);

    return dist(generator);
}

//***************************************************************************************************************************************************
std::string producer::generate_random_programme()
{
    std::uniform_int_distribution<int> dist(0, (int)programmes.size() - 1);

    return programmes[dist(generator)];
}

//***************************************************************************************************************************************************
std::vector<course_mark> producer::generate_random_course_marks()
{
    std::vector<course_mark> course_marks;
    std::uniform_int_distribution<int> num_courses(4, 6); // assume a student can take a range [4, 6] courses
    std::uniform_real_distribution<float> mark(0.0, 100.0);
    std::uniform_int_distribution<int> course_idx(0, (int)course_codes.size() - 1);
    std::vector<std::string> shuffled_cms = course_codes;

    std::shuffle(shuffled_cms.begin(), shuffled_cms.end(), generator);

    const int N = std::min((int)shuffled_cms.size(), num_courses(generator));

    for (int i = 0; i < N; i++)
        course_marks.push_back({ shuffled_cms[i], mark(generator) });

    return course_marks;
}

//***************************************************************************************************************************************************
ITstudent producer::generate_random_student()
{
    return ITstudent(generate_random_first_name(), generate_random_last_name(), generate_random_id(), generate_random_programme(), generate_random_course_marks());
}

//***************************************************************************************************************************************************
void producer::produce(int _file_number)
{
    std::string filename = buffer_.get_shared_directory() + "/student" + std::to_string(_file_number) + ".xml";
    std::ofstream out_file(filename);

    if (out_file.is_open()) 
    {
        ITstudent student = generate_random_student();

        out_file << student.to_XML();
        out_file.close();

        std::cout << "[PRODUCER] Created file: " << filename << "\n";
    }
    else 
    {
        std::cerr << "[PRODUCER] Error: Could not create file " << filename << "\n";
        return;
    }

    buffer_.produce(_file_number); // add a student to buffer
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate production time (to avoid wasting CPU cycles)
}

//***************************************************************************************************************************************************
void producer::run(int _num_students)
{
    std::cout << "[PRODUCER] Starting production of " << _num_students << " students...\n";

    for (int i = 1; i <= _num_students; i++)
        produce(i);

    std::cout << "[PRODUCER] Finished production.\n";
}