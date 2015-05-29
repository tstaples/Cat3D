#ifndef INCLUDED_ENGINE_PATH_H
#define INCLUDED_ENGINE_PATH_H

namespace IO
{

class Path
{
public:
    Path();
    Path(const std::string& path);
    Path(const std::wstring& path);

    std::wstring GetExtension() const;
    std::wstring GetFileName() const;
    std::wstring GetFileNameWithoutExtension() const;

    std::string GetExtensionA() const;
    std::string GetFileNameA() const;
    std::string GetFileNameWithoutExtensionA() const;

    std::wstring GetFullPath() const;
    std::string GetFullPathA() const;

    std::wstring GetPath() const;
    std::string GetPathA() const;

    size_t Length() const;

    Path& operator=(const std::wstring& rhs);
    Path& operator=(const std::string& rhs);

private:
    void ConvertBackslashToForwardslash();
    void ConvertForwardslashToBackslash();

private:
    enum SlashFormat { Back, Forward };

    std::wstring mPath;
    SlashFormat mFormat;
};

} // namespace IO

#endif // #ifndef INCLUDED_ENGINE_PATH_H