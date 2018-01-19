#include <string>
#include "FileWrapper.h"
#include <iostream>

int main()
{
	/*const auto path = std::experimental::filesystem::current_path().append("test.txt");

	std::cout << path.string() << '\n';

	std::ofstream openFile(path.c_str());

	if (openFile.is_open())
	{
		auto lastTimeForWrite = std::experimental::filesystem::last_write_time(path);
		while (true)
		{
			if (auto currentLastTimeForWrite = std::experimental::filesystem::last_write_time(path); std::chrono::time_point_cast<std::chrono::seconds>(lastTimeForWrite) != std::chrono::time_point_cast<std::chrono::seconds>(currentLastTimeForWrite))
			{
				if (currentLastTimeForWrite != std::experimental::filesystem::file_time_type::min())
				{
					const auto time = decltype(currentLastTimeForWrite)::clock::to_time_t(currentLastTimeForWrite);
					std::cout << "File " << path.filename() << " changed at " << std::asctime(std::localtime(&time)) << '\n';
					lastTimeForWrite = currentLastTimeForWrite;
				}
			}
		}
	}
	else
	{
		std::cout << "Error" << std::endl;
	}*/

	std::string fileName("test.txt");
	std::string filePath("");

	try
	{
		/*std::vector<SHADERHOTLOAD::FILESYSTEM::FileWrapper> files;
		files.emplace_back(SHADERHOTLOAD::FILESYSTEM::FileWrapper(fileName, filePath));
		files.emplace_back(SHADERHOTLOAD::FILESYSTEM::FileWrapper("temp.txt", filePath));

		while (true)
		{
			for (auto &file : files)
				if (file.CheckIfUpdated())
				{
					file.UpdateData();
					std::cout << "File: " << file.FileName() << " Changed at: " << file.LastTimeModified() << "File Data: " << std::string(file.GetFileData()) << '\n';
				}
		}*/
		SHADERHOTLOAD::FILESYSTEM::FileWrapper file(fileName, filePath);
		std::cout << "File: " << file.FileName() << " Changed at: " << file.LastTimeModified() << "File Data: " << std::string(file.GetFileData()) << '\n';
		while (true)
		{
			if (file.CheckIfUpdated())
			{
				file.UpdateData();
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