#include "ITstudent.h"
#include <sstream>
#include <iomanip>
#include <iostream>

//***************************************************************************************************************************************************
ITstudent::ITstudent() 
    : first_name("")
    , last_name("")
    , student_id(99999999)
    , programme("")
{}

ITstudent::ITstudent(const std::string& _first_name, const std::string& _last_name, int _student_id, const std::string& _programme, const std::vector<course_mark>& _course_marks)
    : first_name(_first_name)
    , last_name(_last_name)
    , programme(_programme)
    , course_marks(_course_marks)
{
    try
    {
        set_student_id(_student_id);
    }
    catch (const std::invalid_argument& _e)
    {
        std::cout << "Error: " << _e.what() << std::endl;
    }
}

//***************************************************************************************************************************************************
std::string ITstudent::get_full_name() const
{ 
    return first_name + " " + last_name; 
}

//***************************************************************************************************************************************************
std::string ITstudent::get_first_name() const
{ 
    return first_name; 
}

//***************************************************************************************************************************************************
std::string ITstudent::get_last_name() const
{
    return last_name;
}

//***************************************************************************************************************************************************
int ITstudent::get_student_id() const
{
    return student_id;
}

//***************************************************************************************************************************************************
std::string ITstudent::get_programme() const
{
    return programme;
}

//***************************************************************************************************************************************************
std::vector<course_mark> ITstudent::get_course_marks() const
{
    return course_marks;
}

//***************************************************************************************************************************************************
void ITstudent::set_full_name(const std::string& _first_name, const std::string& _last_name)
{ 
    first_name = _first_name;
    last_name = _last_name;
}

//***************************************************************************************************************************************************
void ITstudent::set_first_name(const std::string& _first_name)
{
    first_name = _first_name;
}

//***************************************************************************************************************************************************
void ITstudent::set_last_name(const std::string& _last_name)
{
    last_name = _last_name;
}

//***************************************************************************************************************************************************
void ITstudent::set_student_id(int _student_id)
{
    if (_student_id >= 10000000 && _student_id <= 99999999)
        student_id = _student_id;
    else
        throw std::invalid_argument("Student ID must be EXACTLY 8 digits long.");
}

//***************************************************************************************************************************************************
void ITstudent::set_programme(const std::string& _programme)
{ 
    programme = _programme; 
}

//***************************************************************************************************************************************************
void ITstudent::set_course_marks(const std::vector<course_mark>& _course_marks)
{
    course_marks = _course_marks;
}

//***************************************************************************************************************************************************
float ITstudent::calculate_average() const 
{
    if (course_marks.empty()) 
        return 0.0f;

    float sum = 0.0f;

    for (const auto& course_mark_ : course_marks)
        sum += course_mark_.mark;
    
    return sum / course_marks.size();
}

//***************************************************************************************************************************************************
void ITstudent::print_student_info() const
{
    std::cout << "\n========================================\n";
    std::cout << "Student Name: " << get_full_name() << "\n";
    std::cout << "Student ID: " << student_id << "\n";
    std::cout << "Programme: " << programme << "\n";
    std::cout << "Courses and Marks:\n";

    for (const auto& course_mark_ : course_marks)
        std::cout << "  - " << course_mark_.course_code << ": " << course_mark_.mark << "\n";

    const float avg_mark = calculate_average();

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Average Mark: " << avg_mark << "\n";
    std::cout << "Status: " << ((avg_mark > 50.0f) ? "PASSED" : "FAILED") << "\n";
    std::cout << "========================================\n";
}

//***************************************************************************************************************************************************
std::string ITstudent::to_XML() const
{
    std::ostringstream oss;

    oss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    oss << "<student>\n";
    oss << "  <first_name>" << first_name << "</first_name>\n";
    oss << "  <last_name>" << last_name << "</last_name>\n";
    oss << "  <id>" << student_id << "</id>\n";
    oss << "  <programme>" << programme << "</programme>\n";
    oss << "  <courses>\n";

    for (const auto& course_mark_ : course_marks) 
    {
        oss << "    <course>\n";
        oss << "      <code>" << course_mark_.course_code << "</code>\n";
        oss << "      <mark>" << course_mark_.mark << "</mark>\n";
        oss << "    </course>\n";
    }

    oss << "  </courses>\n";
    oss << "</student>\n";

    return oss.str();
}

//***************************************************************************************************************************************************
ITstudent ITstudent::from_XML(const std::string& _xml)
{
    // we will assume a clean XML with no (unnecessary) whitespaces and escape characters

    ITstudent student;
    auto get_tag_content = [](const std::string& _xml, const std::string& _tag_name) -> std::string {
        std::string start_tag = "<" + _tag_name + ">";
        std::string end_tag = "</" + _tag_name + ">";

        size_t start_pos = _xml.find(start_tag);

        if (start_pos == std::string::npos)
            return "";

        size_t end_pos = _xml.find(end_tag, start_pos);

        if (end_pos == std::string::npos)
            return "";

        start_pos += start_tag.length();

        return _xml.substr(start_pos, end_pos - start_pos);
    };
    
    student.set_first_name(get_tag_content(_xml, "first_name"));
    student.set_last_name(get_tag_content(_xml, "last_name"));

    try
    {
        student.set_student_id(std::stoi(get_tag_content(_xml, "id")));
    }
    catch(const std::invalid_argument& _e)
    {
        std::cout << "Error: " << _e.what() << std::endl;
    }

    student.set_programme(get_tag_content(_xml, "programme"));

    std::string courses_section = get_tag_content(_xml, "courses");
    
    if (courses_section.empty())
        return student;

    size_t course_start = 0;
    std::vector<course_mark> course_marks_;
    const std::string start_course_tag = "<course>", end_course_tag = "</course>";

    while ((course_start = courses_section.find(start_course_tag, course_start)) != std::string::npos)
    {
        size_t course_end = courses_section.find(end_course_tag, course_start);

        if (course_end == std::string::npos)
            break;

        std::string course_block = courses_section.substr(course_start + start_course_tag.length(), course_end - course_start - start_course_tag.length());

        course_marks_.push_back({ get_tag_content(course_block, "code"), std::stof(get_tag_content(course_block, "mark")) });
        course_start = course_end + start_course_tag.length();
    }

    student.set_course_marks(course_marks_);

    return student;
}