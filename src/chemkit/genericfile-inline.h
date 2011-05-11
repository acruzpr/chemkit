/******************************************************************************
**
** Copyright (C) 2009-2011 Kyle Lutz <kyle.r.lutz@gmail.com>
** All rights reserved.
**
** This file is a part of the chemkit project. For more information
** see <http://www.chemkit.org>.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in the
**     documentation and/or other materials provided with the distribution.
**   * Neither the name of the chemkit project nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
******************************************************************************/

#ifndef CHEMKIT_GENERICFILE_INLINE_H
#define CHEMKIT_GENERICFILE_INLINE_H

#include "genericfile.h"

#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <QFile>

namespace chemkit {

// === GenericFile ========================================================= //
/// \class GenericFile genericfile.h chemkit/genericfile.h
/// \ingroup chemkit
/// \brief The GenericFile class represents a generic file.
///
/// The GenericFile class provides a common interface for interacting
/// with chemical data files.

// --- Construction and Destruction ---------------------------------------- //
/// Creates a new file.
template<typename File, typename Format>
inline GenericFile<File, Format>::GenericFile()
    : m_format(0)
{
}

/// Creates a new file with \p fileName.
template<typename File, typename Format>
inline GenericFile<File, Format>::GenericFile(const std::string &fileName)
    : m_format(0)
{
    setFileName(fileName);
}

/// Destroys the file.
template<typename File, typename Format>
inline GenericFile<File, Format>::~GenericFile()
{
    delete m_format;
}

// --- Properties ---------------------------------------------------------- //
/// Sets the file name for the file to \p fileName.
///
/// If no file format is set the suffix of \p fileName will be
/// used as the format.
template<typename File, typename Format>
inline void GenericFile<File, Format>::setFileName(const std::string &fileName)
{
    m_fileName = fileName;

    if(!m_format){
        setFormat(suffix(fileName));
    }
}

/// Returns the file name for the file.
template<typename File, typename Format>
inline std::string GenericFile<File, Format>::fileName() const
{
    return m_fileName;
}

/// Sets the format to \p format.
template<typename File, typename Format>
inline void GenericFile<File, Format>::setFormat(Format *format)
{
    // delete old format
    delete m_format;

    // set new format
    m_format = format;
}

/// Sets the format for the file to \p formatName. Returns \c false
/// if \p formatName is not supported.
template<typename File, typename Format>
inline bool GenericFile<File, Format>::setFormat(const std::string &formatName)
{
    Format *format = Format::create(formatName);
    if(!format){
        setErrorString((boost::format("File format '%s' is not supported.") % formatName).str());
        return false;
    }

    setFormat(format);

    return true;
}

/// Returns the format object for the file.
template<typename File, typename Format>
inline Format* GenericFile<File, Format>::format() const
{
    return m_format;
}

/// Returns the name of the format for the file or an empty string
/// if no format is set.
template<typename File, typename Format>
inline std::string GenericFile<File, Format>::formatName() const
{
    if(m_format){
        return m_format->name();
    }

    return std::string();
}

// --- Input and Output ---------------------------------------------------- //
/// Reads the file using the current file name. Returns \c false if
/// no file name is set or if reading of the file fails.
template<typename File, typename Format>
inline bool GenericFile<File, Format>::read()
{
    if(m_fileName.empty()){
        setErrorString("No file name set for reading.");
        return false;
    }
    else if(m_format == 0){
        setErrorString("No file format set for reading.");
        return false;
    }

    // open file
    QFile file(m_fileName.c_str());
    bool ok = file.open(QFile::ReadOnly);
    if(!ok){
        setErrorString("Failed to open file for reading.");
        return false;
    }

    return read(&file);
}

/// Reads the file from \p fileName. Returns \c false if reading
/// the file fails.
///
/// Equivalent to:
/// \code
/// file.setFileName(fileName);
/// file.read();
/// \endcode
template<typename File, typename Format>
inline bool GenericFile<File, Format>::read(const std::string &fileName)
{
    return read(fileName, suffix(fileName));
}

/// Reads the file from \p fileName with \p formatName. Returns
/// \c false if \p formatName is not supported or if reading the file
/// fails.
///
/// Equivalent to:
/// \code
/// file.setFileName(fileName);
/// file.setFormat(formatName);
/// file.read();
/// \endcode
template<typename File, typename Format>
inline bool GenericFile<File, Format>::read(const std::string &fileName, const std::string &formatName)
{
    // set the file name for the file
    setFileName(fileName);

    // set the format for the file
    bool ok = setFormat(formatName);
    if(!ok){
        return false;
    }

    return read();
}

/// Reads the file from \p iodev using \p formatName. Returns
/// \c false if \p formatName is not supported or if reading the file
/// fails.
///
/// Equivalent to:
/// \code
/// file.setFormat(formatName);
/// file.read(iodev);
/// \endcode
template<typename File, typename Format>
inline bool GenericFile<File, Format>::read(QIODevice *iodev, const std::string &formatName)
{
    // set the format for the file
    bool ok = setFormat(formatName);
    if(!ok){
        return false;
    }

    return read(iodev);
}

/// Reads the file from \p iodev. Returns \c false if reading the
/// file fails.
template<typename File, typename Format>
inline bool GenericFile<File, Format>::read(QIODevice *iodev)
{
    // check for valid format
    if(!m_format){
        setErrorString("No file format set for reading.");
        return false;
    }

    // read the file
    bool ok = m_format->read(iodev, static_cast<File *>(this));
    if(!ok){
        setErrorString(m_format->errorString());
    }

    return ok;
}

/// Writes to the file using the set file name. Returns \c false if
/// writing the file fails.
template<typename File, typename Format>
inline bool GenericFile<File, Format>::write()
{
    if(m_fileName.empty()){
        setErrorString("No file name set for writing.");
        return false;
    }

    return write(m_fileName);
}

/// Writes to the file with \p fileName using its suffix as the
/// format. Returns \c false if writing the file fails.
template<typename File, typename Format>
inline bool GenericFile<File, Format>::write(const std::string &fileName)
{
    return write(fileName, suffix(fileName));
}

/// Writes to the file with \p fileName using \p formatName. Returns
/// \c false if \p formatName is not supported or if writing the file
/// fails.
template<typename File, typename Format>
inline bool GenericFile<File, Format>::write(const std::string &fileName, const std::string &formatName)
{
    QFile file(fileName.c_str());
    if(!file.open(QIODevice::WriteOnly)){
        setErrorString((boost::format("Failed to open '%s' for writing: %s.") % fileName % file.errorString().toStdString()).str());
        return false;
    }

    return write(&file, formatName);
}

/// Writes the file to \p iodev using \p formatName. Returns \c false
/// if \p formatName is not supported or if writing the file fails.
template<typename File, typename Format>
inline bool GenericFile<File, Format>::write(QIODevice *iodev, const std::string &formatName)
{
    Format *format = Format::create(formatName);
    if(!format){
        setErrorString((boost::format("File format '%s' is not supported.") % formatName).str());
        return false;
    }

    return write(iodev, format);
}

/// Writes the file to \p iodev using the set format. Returns
/// \c false if writing the file fails.
template<typename File, typename Format>
inline bool GenericFile<File, Format>::write(QIODevice *iodev)
{
    return write(iodev, m_format);
}

/// Writes the file to \p iodev using \p format. Returns \c false if
/// writing the file fails.
template<typename File, typename Format>
inline bool GenericFile<File, Format>::write(QIODevice *iodev, Format *format)
{
    if(!format){
        setErrorString("No format set for writing.");
        return false;
    }

    return format->write(static_cast<const File *>(this), iodev);
}

// --- File Data ----------------------------------------------------------- //
/// Sets data with \p name to \p value for the file.
template<typename File, typename Format>
inline void GenericFile<File, Format>::setData(const std::string &name, const Variant &value)
{
    m_data[name] = value;
}

/// Returns the data value with \p name for the file.
template<typename File, typename Format>
inline Variant GenericFile<File, Format>::data(const std::string &name) const
{
    std::map<std::string, Variant>::const_iterator iter = m_data.find(name);
    if(iter != m_data.end()){
        return iter->second;
    }

    return Variant();
}

// --- Error Handling ------------------------------------------------------ //
/// Sets a string describing the last error that occurred.
template<typename File, typename Format>
inline void GenericFile<File, Format>::setErrorString(const std::string &errorString)
{
    m_errorString = errorString;
}

/// Returns a string describing the last error that occurred.
template<typename File, typename Format>
inline std::string GenericFile<File, Format>::errorString() const
{
    return m_errorString;
}

// --- Static Methods ------------------------------------------------------ //
/// Returns a list of all the supported file formats.
template<typename File, typename Format>
inline std::vector<std::string> GenericFile<File, Format>::formats()
{
    return Format::formats();
}

// --- Internal Methods ---------------------------------------------------- //
/// Returns the file suffix for the file name.
template<typename File, typename Format>
inline std::string GenericFile<File, Format>::suffix(const std::string &fileName)
{
    boost::filesystem::path extension = boost::filesystem::extension(fileName);
    if(extension.empty()){
        return std::string();
    }

    std::string suffix = extension.string();

    // remove the leading '.' character
    suffix.erase(0, 1);

    return suffix;
}

} // end chemkit namespace

#endif // CHEMKIT_GENERICFILE_INLINE_H
