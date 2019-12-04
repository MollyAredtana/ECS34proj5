#ifndef PATH_H 	  			 	 
#define PATH_H

#include <string>
#include <memory>

class CPath{
    protected:
        struct SImplementation;
        std::unique_ptr< SImplementation > DImplementation;
        
        static std::string GetCWD();
        static std::string GetHome();
        
    public:
        CPath();
        CPath(const CPath &path);
        CPath(const std::string &path);
        ~CPath();
        
        CPath &operator=(const CPath &path);
        
        CPath operator+(const CPath &path) const;
        bool operator==(const CPath &path) const;
        bool operator!=(const CPath &path) const;
        
        CPath Directory() const;
        std::string BaseName() const;
        std::string Extension() const;
        
        bool IsAbsolute() const;
        
        std::string ToString() const;
        operator std::string() const;
        
        CPath AbsolutePath() const;
        CPath CommonPath(const CPath &path) const;
        CPath NormalizePath() const;
        CPath RelativePathTo(const CPath &path) const;
        
        static CPath AbsolutePath(const CPath &path);
        static CPath CurrentPath();
        static CPath CommonPath(const CPath &path1, const CPath &path2);
        static CPath ExpandUserPath(const CPath &path);
        static CPath HomePath();
        static CPath NormalizePath(const CPath &path);
        static CPath RelativePath(const CPath &path, const CPath &startpath = CurrentPath());
};

#endif
