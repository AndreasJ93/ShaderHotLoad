#ifndef FILE_WRAPPER_H
#define FILE_WRAPPER_H

#include <fstream>
#include <filesystem>

//Todo list
//#Go through the code, look at what can be placed in a .cpp file.

namespace SHADERHOTLOAD::FILESYSTEM
{
	namespace STDFileSystem = std::experimental::filesystem;
	namespace STDChrono = std::chrono;
	class FileWrapper
	{
	private:
		//Typedefs//
		typedef std::ifstream FileOpener;
		typedef STDFileSystem::path FilePath;
		typedef STDFileSystem::v1::file_time_type FileTimeType;
		typedef STDChrono::time_point<std::chrono::system_clock> FileTime;

		//Variables//
		FilePath myPath;
		FileOpener myFile;
		mutable std::string myFileData;
		mutable FileTime lastWriteTime; //This will be changeable! This is never a concer for the outside code
		mutable std::time_t lastWriteTime_t; //See above!
		mutable bool updatedSinceLastRead;

		//Helper Functions//
		inline FileTime LastWriteTime() const { return STDFileSystem::last_write_time(myPath); };
		inline auto ConvertToSeconds(const FileTime &time) const
		{
			return STDChrono::time_point_cast<STDChrono::seconds>(time);
		};
		inline bool TimeCompare(const FileTime &currentTime, const FileTime &newTime) const
		{
			return ConvertToSeconds(currentTime) == ConvertToSeconds(newTime);
		};
		std::string GetLineOfFile()
		{
			/*Found at https://stackoverflow.com/questions/6089231/getting-std-ifstream-to-handle-lf-cr-and-crlf */
			std::string temp;
			std::streambuf* streamBuffer = myFile.rdbuf();
			for (;;) {
				const int c = streamBuffer->sbumpc();
				switch (c) {
				case '\n':
					return temp;
				case '\r':
					if (streamBuffer->sgetc() == '\n')
						streamBuffer->sbumpc();
					return temp;
				case std::streambuf::traits_type::eof():
					// Also handle the case when the last line has no line ending
					if (temp.empty())
						myFile.setstate(std::ios::eofbit);
					return temp;
				default:
					temp += static_cast<char>(c);
				}
			}
		}
		inline void ReadDataFromFile()
		{
			myFile.clear();
			myFile.seekg(0, std::ios_base::beg);
			if (myFile.eof())
				std::exception("The file is empty.");

			//Clear the string, but expect at least as much data as before.
			const size_t sizeBeforeClear = myFileData.size();
			myFileData.clear();
			myFileData.reserve(sizeBeforeClear);
			while (!myFile.eof())
				myFileData += GetLineOfFile() + '\n';
			while (!myFileData.empty() && myFileData.back() == '\n')
				myFileData.pop_back(); //Remove the extra '\n'!
		}

	public:
		//Creates a FileWrapper (read only!) for the specified file. If no path is defined, workingDir will be used!
		FileWrapper(const std::string_view &file, const std::string_view &directoryPath) throw(std::exception) :
			updatedSinceLastRead(false)
		{
			std::string path(directoryPath);
			if (file.empty())
				throw std::exception("No file provided.");
			if (path.empty())
				path = STDFileSystem::current_path().string();
			path += "\\";
			path += file;

			myPath = FilePath(path);
			myFile.open(myPath, std::ios::in);
			if (!myFile.is_open())
				throw std::exception("Could not open file.");

			lastWriteTime = LastWriteTime();
			lastWriteTime_t = FileTime::clock::to_time_t(lastWriteTime);

			ReadDataFromFile();
		}

		bool CheckIfUpdated() const
		{
			const auto currentWriteTime = LastWriteTime();
			if (!TimeCompare(lastWriteTime, currentWriteTime))
			{
				lastWriteTime = currentWriteTime;
				lastWriteTime_t = FileTime::clock::to_time_t(lastWriteTime);
				updatedSinceLastRead = true;
				return true;
			}
			return false;
		}

		char const* LastTimeModified() const
		{
			return std::asctime(std::localtime(&lastWriteTime_t));
		}

		std::string FileName() const
		{
			return myPath.filename().string();
		}

		std::string PathToFile() const
		{
			return myPath.string();
		}

		std::string_view GetFileData()
		{
			if (updatedSinceLastRead)
			{
				ReadDataFromFile();
				updatedSinceLastRead = false;
			}
			return myFileData;
		}
	};
}

#endif