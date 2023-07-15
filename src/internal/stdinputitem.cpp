// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
 * bit7z - A C++ static library to interface with the 7-zip shared libraries.
 * Copyright (c) 2014-2023 Riccardo Ostani - All Rights Reserved.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "internal/stdinputitem.hpp"

#include "internal/cstdinstream.hpp"
#include "internal/dateutil.hpp"
#include "internal/util.hpp"

using bit7z::StdInputItem;
using bit7z::tstring;
using std::istream;

StdInputItem::StdInputItem( istream& stream, const tstring& path ) : mStream{ stream }, mStreamPath{ path } {}

auto StdInputItem::name() const -> tstring {
    return mStreamPath.filename().string< tchar >();
}

auto StdInputItem::path() const -> tstring {
    return mStreamPath.string< tchar >();
}

auto StdInputItem::inArchivePath() const -> fs::path {
    return mStreamPath;
}

auto StdInputItem::getStream( ISequentialInStream** inStream ) const -> HRESULT {
    auto inStreamLoc = bit7z::make_com< CStdInStream, ISequentialInStream >( mStream );
    *inStream = inStreamLoc.Detach(); //Note: 7-zip will take care of freeing the memory!
    return S_OK;
}

auto StdInputItem::isDir() const noexcept -> bool {
    return false;
}

auto StdInputItem::size() const -> uint64_t {
    const auto original_pos = mStream.tellg();
    mStream.seekg( 0, std::ios::end ); // seeking to the end of the stream
    const auto result = static_cast< uint64_t >( mStream.tellg() - original_pos ); // size of the stream
    mStream.seekg( original_pos ); // seeking back to the original position in the stream
    return result;
}

auto StdInputItem::creationTime() const noexcept -> FILETIME { //-V524
    return currentFileTime();
}

auto StdInputItem::lastAccessTime() const noexcept -> FILETIME { //-V524
    return currentFileTime();
}

auto StdInputItem::lastWriteTime() const noexcept -> FILETIME {
    return currentFileTime();
}

auto StdInputItem::attributes() const noexcept -> uint32_t {
    return static_cast< uint32_t >( FILE_ATTRIBUTE_NORMAL );
}
