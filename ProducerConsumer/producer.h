#pragma once

#include "buffer.h"
#include "ITstudent.h"
#include <random>

//***************************************************************************************************************************************************
class producer
{
public:
    //***********************************************************************************************************************************************
    producer(buffer& _buffer);

    std::string generate_random_first_name();
    std::string generate_random_last_name();
    int generate_random_id();
    std::string generate_random_programme();
    std::vector<course_mark> generate_random_course_marks();
    ITstudent generate_random_student();

    void produce(int _file_number);
    void run(int _num_students);

private:
    //***********************************************************************************************************************************************
    buffer& buffer_;
    std::mt19937 generator;
    
    const std::vector<std::string> first_names = {"Sifiso", "Muzi", "James", "Pertunia", "Clementine", "Lindani", "Busiswa", "Barbara", "Elizabeth" };
    const std::vector<std::string> last_names = { "Smith", "Fakudze", "Xulu", "Shiba", "Jones", "Garcia", "Mnisi", "Davis", "Ngcamphalala", "Martinez" };
    const std::vector<std::string> programmes = { "BSc Computer Science", "BSc Information Technology", "BSc Software Engineering", "BSc Data Science" };
    const std::vector<std::string> course_codes = { "CSC231", "CSC123", "CSC456", "CSC321", "CSC333", "CSC411", "CSC312" };
};