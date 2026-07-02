#include "../includes/render/shader.h"
#include "../includes/config.h"

// credit to: GetIntoGameDev for shader compilation methods
// None of the following code was modified

Shader::Shader(const std::string &vertex_filepath, const std::string &fragment_filepath)
{
    std::vector<unsigned int> modules;
    modules.push_back(make_shader_module(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(make_shader_module(fragment_filepath, GL_FRAGMENT_SHADER));

    // creating shader program
    unsigned int shader_program = glCreateProgram();

    for (unsigned int shader_module : modules)
    {
        glAttachShader(shader_program, shader_module);
    }

    glLinkProgram(shader_program);

    // sometimes linking shaders fails, so we need to check if compilation was successful
    int link_success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &link_success);
    if (!link_success)
    {
        char errorLog[1024];
        glGetProgramInfoLog(shader_program, 1024, NULL, errorLog);
        std::cout << "Shader Linking Error:\n"
                << errorLog << std::endl;
    }

    // deleting shader modules (no longer needed)
    for (unsigned int shader_module : modules)
    {
        glDeleteShader(shader_module);
    }

    this -> shaderID = shader_program;
}

unsigned int Shader::make_shader_module(const std::string &filePath, unsigned int module_type)
{
    // getting data from file and converting to string
    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filePath);

    while (std::getline(file, line))
    {
        bufferedLines << line << "\n";
    }

    std::string shaderSource = bufferedLines.str();

    const char *shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    // compiling shader module
    unsigned int shader_module = glCreateShader(module_type);
    glShaderSource(shader_module, 1, &shaderSrc, NULL);
    glCompileShader(shader_module);

    // sometimes compiling shader fails, so we need to check if compilation was successful
    int compSuccess;
    glGetShaderiv(shader_module, GL_COMPILE_STATUS, &compSuccess);
    if (!compSuccess)
    {
        char errorLog[1024];
        glGetShaderInfoLog(shader_module, 1024, NULL, errorLog);
        std::cout << "Shader Module Compilation Error:\n"
                << errorLog << std::endl;
    }

    return shader_module;
}

void Shader::set_int(std::string uniform, int value)
{
    int loc = glGetUniformLocation(shaderID, uniform.c_str());
    glUniform1i(loc, value);
}

void Shader::set_float(std::string uniform, float value)
{
    int loc = glGetUniformLocation(shaderID, uniform.c_str());
    glUniform1f(loc, value);
}

void Shader::set_mat4(std::string uniform, glm::mat4 value)
{
    int loc = glGetUniformLocation(shaderID, uniform.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}