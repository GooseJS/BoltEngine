#pragma once

#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "BoltEngine/GL.h"
#include "BoltEngine/Core.h"
#include "BoltEngine/logger/Logger.h"

namespace Bolt
{
	class BOLT_API Shader
	{
	private:
		enum class ShaderObjectType
		{
			SHADER, PROGRAM
		};

		GLuint _programID;
		GLuint _vertexShader;
		GLuint _fragmentShader;

		std::map<std::string, int> _attributeMap;
		std::map<std::string, int> _uniformMap;

		bool _init;

		GLuint compileShader(std::string shaderSource, GLenum shaderType)
		{
			std::string shaderTypeString;
			switch (shaderType)
			{
			case GL_VERTEX_SHADER:
				shaderTypeString = "GL_VERTEX_SHADER";
				break;
			case GL_FRAGMENT_SHADER:
				shaderTypeString = "GL_FRAGMENT_SHADER";
				break;
			case GL_GEOMETRY_SHADER:
				shaderTypeString = "GL_GEOMETRY_SHADER";
				break;
			default:
				shaderTypeString = "ERROR";
				BOLT_ENGINE_CRITICAL("Invalid shader type \"{}\" given when compiling shader.", shaderType);
				return -1;
			}

			GLuint shaderID = glCreateShader(shaderType);
			if (shaderID == 0)
			{
				BOLT_ENGINE_CRITICAL("Error creating shader. glCreateShader({}) returned 0.", shaderTypeString); // TODO: Handle this better?
				return -1;
			}

			const char* shaderSource_cstr = shaderSource.c_str();
			glShaderSource(shaderID, 1, &shaderSource_cstr, nullptr);
			glCompileShader(shaderID);

			GLint shaderStatus;
			glGetShaderiv(shaderID, GL_COMPILE_STATUS, &shaderStatus);
			if (shaderStatus == GL_FALSE)
			{
				BOLT_ENGINE_CRITICAL("Shader copilation failed. Type: {} - Error: {}", shaderTypeString, getInfoLog(ShaderObjectType::SHADER, shaderID));
				return -1;
			}

			return shaderID;
		}

		void init(std::string vertexShaderSource, std::string fragmentShaderSource)
		{
			_programID = glCreateProgram();
			glUseProgram(_programID);

			_vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
			_fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

			glAttachShader(_programID, _vertexShader);
			glAttachShader(_programID, _fragmentShader);

			glLinkProgram(_programID);

			glDetachShader(_programID, _vertexShader);
			glDetachShader(_programID, _fragmentShader);

			GLint programLinkStatus = GL_FALSE;
			glGetProgramiv(_programID, GL_LINK_STATUS, &programLinkStatus);
			if (programLinkStatus == GL_FALSE)
			{
				BOLT_ENGINE_CRITICAL("Shader link failed. Error: {}", getInfoLog(ShaderObjectType::PROGRAM, _programID));
				return;
			}

			glValidateProgram(_programID);

			GLint programValidationStatus = GL_FALSE;
			glGetProgramiv(_programID, GL_VALIDATE_STATUS, &programValidationStatus);
			if (programValidationStatus == GL_FALSE)
			{
				BOLT_ENGINE_CRITICAL("Shader validation failed. Error: {}", getInfoLog(ShaderObjectType::PROGRAM, _programID));
				return;
			}

			_init = true;
		}

		std::string getInfoLog(ShaderObjectType type, GLuint id)
		{
			GLint infoLogLength;
			if (type == ShaderObjectType::SHADER)
			{
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
			}
			else
			{
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
			}

			GLchar* infoLog = new GLchar[infoLogLength + 1];
			if (type == ShaderObjectType::SHADER)
			{
				glGetShaderInfoLog(id, infoLogLength, nullptr, infoLog);
			}
			else
			{
				glGetProgramInfoLog(id, infoLogLength, nullptr, infoLog);
			}
			std::string infoLogString(infoLog);

			delete[] infoLog;

			return infoLogString;
		}

	public:
		Shader() : _init(false) {}

		~Shader() { glDeleteProgram(_programID); }

		inline void initFromString(std::string vertexShader, std::string fragmentShader)
		{
			init(std::move(vertexShader), std::move(fragmentShader));
		}

		inline void initFromFile(std::string vertexShaderLoc, std::string fragmentShaderLoc)
		{
			std::ifstream vertexFileStream(vertexShaderLoc);
			std::ifstream fragmentFileStream(fragmentShaderLoc);

			if (!vertexFileStream.is_open())
			{
				BOLT_ENGINE_CRITICAL("Could not open Vertex Shader File! Location: {}", vertexShaderLoc);
				return;
			}

			if (!fragmentFileStream.is_open())
			{
				BOLT_ENGINE_CRITICAL("Could not open Fragment Shader File! Location: {}", fragmentShaderLoc);
				return;
			}

			std::string vertexResult;
			std::string fragmentResult;
			std::string line;
			while (!vertexFileStream.eof())
			{
				std::getline(vertexFileStream, line);
				vertexResult.append(line + "\n");
			} vertexFileStream.close();
			while (!fragmentFileStream.eof())
			{
				std::getline(fragmentFileStream, line);
				fragmentResult.append(line + "\n");
			} fragmentFileStream.close();

			initFromString(std::move(vertexResult), std::move(fragmentResult));
		}

		inline void use() const
		{
			if (_init)
				glUseProgram(_programID);
			else
				BOLT_ENGINE_CRITICAL("Program must be initialized before being used.");
		}

		GLuint attribute(std::string attribName)
		{
			auto iter = _attributeMap.find(attribName);
			if (iter == _attributeMap.end())
			{
				const char* attribName_cstr = attribName.c_str();
				GLint attribLookup = glGetAttribLocation(_programID, attribName_cstr);
				if (attribLookup == -1)
				{
					BOLT_ENGINE_CRITICAL("Could not find shader attribute: {}", attribName);
					return -1;
				}
				else
				{
					_attributeMap[attribName] = attribLookup;
					return attribLookup;
				}
			}
			return iter->second;
		}

		GLuint uniform(std::string uniformName) // TODO: Change this class over to better const
		{
			auto iter = _uniformMap.find(uniformName);
			if (iter == _uniformMap.end())
			{
				const char* uniformName_cstr = uniformName.c_str();
				GLint uniformLookup = glGetUniformLocation(_programID, uniformName_cstr);
				if (uniformLookup == -1)
				{
					BOLT_ENGINE_CRITICAL("Could not find shader uniform: {}", uniformName);
					return -1;
				}
				else
				{
					_uniformMap[uniformName] = uniformLookup;
					return uniformLookup;
				}
			}
			return iter->second;
		}

		void uniform(std::string uniformName, glm::mat4& matrix)
		{
			use();
			glUniformMatrix4fv(uniform(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
		}
	};
}