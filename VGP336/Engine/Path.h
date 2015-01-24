#ifndef INCLUDED_ENGINE_PATH_H
#define INCLUDED_ENGINE_PATH_H

class Path
{
public:
    Path();
    Path(const std::string& path);
    Path(const std::wstring& path);

    std::wstring GetExtension() const;
    std::wstring GetFileName() const;
    std::wstring GetFileNameWithoutExtension() const;

    std::wstring GetFullPath() const;

    std::wstring GetPath() const;
    std::string GetPathString() const;

    size_t Length() const;

private:
    void ConvertBackslashToForwardslash();
    void ConvertForwardslashToBackslash();

private:
    enum SlashFormat { Back, Forward };

    std::wstring mPath;
    SlashFormat mFormat;
};

#endif // #ifndef INCLUDED_ENGINE_PATH_H