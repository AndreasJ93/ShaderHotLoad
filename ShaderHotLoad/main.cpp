#include <string>
#include "FileWrapper.h"
#include <iostream>
#include <windows.h>

//Todo List:
//#Open a window
//#Create the basic Dx11 Framework (nothing fancy for now)
//#Connect ImGUI
//#Create a basic structure for "hotloading" shader code

int main()
{
	std::string fileName("test.txt");
	std::string filePath("");

	try
	{
		SHADERHOTLOAD::FILESYSTEM::FileWrapper file(fileName, filePath);
		std::cout << "File: " << file.FileName() << " Changed at: " << file.LastTimeModified() << "File Data: " << std::string(file.GetFileData()) << '\n';
		while (true)
		{
			if (file.CheckIfUpdated())
			{
				Sleep(2);
				std::cout << "File: " << file.FileName() << " Changed at: " << file.LastTimeModified() << "File Data: " << std::string(file.GetFileData()) << '\n';
			}
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << '\n';
	}

	return 0;
}