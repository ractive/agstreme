# Third-party code and assets in AGStreme

AGStreme itself is licensed under the GNU General Public License v2 (see
`LICENSE`). The following third-party components are included in or required by
the source tree. Licence statements are summarised from the headers of the files
in question; the headers themselves are authoritative.

## Included in this repository

| Component | Files | Author / origin | Licence (as stated in the files) |
|---|---|---|---|
| id3lib 3.8.0pre1 (ID3v1/ID3v2 tag library) | `id3lib/` | id3lib project, SourceForge (Dirk Mahoney, Scott Thomas Haug et al.) | GNU Library General Public License v2 (`id3lib/COPYING`) |
| zlib 1.1.3 | `id3lib/zlib/` | Jean-loup Gailly and Mark Adler | zlib licence (`id3lib/zlib/COPYING`) |
| CSizingControlBar 2.43 (`sizecbar.*`, `scbarcf.*`, `scbarg.*`) | repository root | Cristi Posea, www.datamekanix.com | "This code is free for personal and commercial use, providing this notice remains intact in the source files and all eventual changes are clearly marked with comments. You must obtain the author's consent before you can include this code in a software library." |
| CHyperLink (`HyperLink.h/.cpp`) | repository root | Chris Maunder, 1997-1998 | "Feel free to use and distribute. May not be sold for profit." |
| CRegBinding (`RegBinding.h/.cpp`) | repository root | Joerg Dentler, 1998 | Permissive four-clause notice: free to use and redistribute for any purpose; origin must not be misrepresented; altered versions must be marked; notice must not be removed. |
| CTrayIcon (`TrayIcon.h/.cpp`) | repository root | Paul DiLascia, Microsoft Systems Journal C/C++ Q&A (1995/1996) | MSJ sample code, as stated in the file header: "stolen from the MSDN article C/C++ Q&A by Paul DiLascia". No explicit licence; included here under the GPL wrapper header the author of AGStreme added. |
| CStatusControl / CStatusProgress (`StatusControl.*`, `StatusProgress.*`) | repository root | Adapted from "Showing progress bar in a status bar pane" by Brad Mann, amended by Thomas Stadler (CodeGuru) | No licence text in file; CodeGuru sample code. |
| CMP3HeaderInfo (`MP3HeaderInfo.*`) | repository root | Based on Cedric Tefft's mp3info (http://ibiblio.org/mp3info/), GPL; `GetHeaderSizeResync`/`GetID3HeaderSize` attributed to "Mantikor" | mp3info is GPL-licensed; encapsulated into a class by the AGStreme author. |
| Winamp frontend/plug-in control API (`FRONTEND.H`) | repository root | Justin Frankel / Christophe Thibault, Copyright (C) 1997-2000 Nullsoft Inc. | Winamp SDK header, distributed by Nullsoft for writing Winamp front-ends. No licence text in the file. |
| SOCKS constants (`sharedConstants.h`) | repository root | Audiogalaxy satellite protocol constants (origin not stated in file) | No licence text in file. |
| Pete's AVI collection (`res/searchtree.avi`, `res/address book copy.avi`) | `res/` | "Pete's AVI files", a freely distributed collection of Windows animation clips | Distributed free for use in applications. |
| ID3v2 logo (`help/id3v2.gif`) | `help/` | id3.org | Logo of the ID3v2 standard. |

## Required but NOT included

| Component | Why it is needed | Why it is not here |
|---|---|---|
| Gracenote CDDB Control SDK (`CDDBControl.dll` and the compiler-generated `CDDBControl.tlh`/`.tli`) | `CDDBInfo.cpp` uses `#import` of the CDDB COM control to look up album information. | Proprietary Gracenote (formerly CDDB Inc.) SDK; redistribution not permitted. The per-developer client ID and client tag have been replaced by placeholders in `CDDBInfo.cpp` and `MainFrm.cpp`. The Gracenote CDDB service for this control was discontinued. |
| Microsoft DAO 3.5 (`DAOInst.zip` installer) | The music database (`DB/songs.mdb`, `*Set.cpp` recordsets) uses MFC DAO classes. | Microsoft redistributable; not ours to publish. The Access database file `songs.mdb` is also excluded because it contained the author's personal music collection. |
| Audiogalaxy Satellite (`AGSatellite.exe`) | AGStreme is a front-end that drives the official satellite. | Proprietary Audiogalaxy binary; the service shut down in 2002. |
| Microsoft Visual C++ 6.0 / MFC | Build toolchain. | Commercial. |

## Excluded Paint Shop Pro sources

`logo.psp`, `splash.psp` and `wizard.psp` (Paint Shop Pro working files for the
bitmaps in `res/`) were left out of the repository; the exported bitmaps are
included.
