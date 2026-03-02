#ifndef TESTS_H
#define TESTS_H

#include <vector>
#include <string>
#include <memory>

class Test
{
public:
    virtual ~Test() = default;
    virtual void test() = 0;
};

class ConnectionTest : public Test
{
public:
    ConnectionTest() = default;
    ~ConnectionTest() = default;
    
    void set_params(const std::vector<std::string>& params);
    void test() override;

private:
    std::vector<std::string> addresses_;
    
    void validate_ip(const std::string& ip) const;
    void validate_port(const std::string& port) const;
};

class ResourceTest : public Test
{
public:
    ResourceTest() = default;
    ~ResourceTest() = default;
    
    void set_params(const std::vector<std::string>& params);
    void test() override;

private:
    std::vector<std::string> paths_;
    
    void validate_file_exists(const std::string& path) const;
};

#endif