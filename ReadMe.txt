======================================================================================
                                   AGStreme Readme
                                      (aka JAGS)
======================================================================================

AGStreme Version 2.0

I renamed the program to AGStreme, because there's already a project that uses the
name JAGS.

What's that?
======================================================================================

Napster's dead! Long live www.audiogalaxy.com!

Audiogalaxy is a sharing-platform for MP3 files with a HUGE community. You find almost
every kind of song.

The idea how to handle file up- and downloads is in contrast to other sharing-platforms 
(like Napster (R.I.P.), kazaa, iMesh etc.) a bit different. The whole service is
web-based. You search your songs on audiogalaxy's homepage and add them to your
download queue within the webform.
The client on your PC will then send and receive requested files.

The main reason why I started with AGStreme is the fact, that the original client is not
really "state of the art" software: too many bugs and too less comfort!

I hope AGStreme will improve the whole audiogalaxy feeling :-)



LICENSE
======================================================================================
AGStreme is licensed under the GNU General Public License (GPL) and is therefore
freely distributable. You can get the source-code at:
http://www.ractive.ch/gpl/AGStreme.html

INSTALLATION
======================================================================================
Run the installation program AGStreme.exe :-) You can either install it
in the directory where your previous installation of the audiogalaxy client was or
choose a new place. If you install it in the old directory, the same settings will be
used! 


If you install the program into a differen directory you may want to copy the files

   config
   shares.txt
   settings

into the new installation directory. This way wou can save your shares, settings etc.


To start the the program you have to call AGSatellite.exe (not AGStreme.exe!), but the 
installation program will set this up for you!

PROBLEMS
======================================================================================
- Some users reported that AGStreme does not even start. I have no clue, why!?!?!
  Please tell me if it works for you (yes, also then) or not!
  Include the version of your OS and Internet Explorer if possible, please!

- when your start the program for the first time, everything looks quite messy.
  Drag the panes to where you want them. I will see what I can do that a useful
  window configuration is automatically set up.
  If you hold down CTRL while dragging the panes, they won't dock.
  If you hold SHIFT, the will dock to the left/right side

- Sometimes when the transfer meter is shown, the main window can't be restored from
  the taskbar (by clicking on the window button). To resolve this problem, click on the
  tray-icon!

- If you create a new directory in a folder that has no subdirectories yet, the
  new item may be out of scope :-(

- You can't set the cursor in a tree of list if you are renaming an item

If you find bugs or if you miss a feature, contact me (see below).


CHANGE-LOG
======================================================================================
Version 2.0
====================
- New name AGStreme
- Music database implemented
- Blocked songs are shown
- CDDB integration
- ID3v2 tag support
- "Album search" added
- much more....

Version 1.1
====================
- Full Drag and Drop support
- Context-Menu support
- Search for song lyrics added
- Minor bug-fixes and other improvements

Version 1.0
====================
- The "MP3-files" pane is automatically updated
- Bug-fixes

Version 0.9
====================
- Better handling if no configuration files are present
- Context menu handling in the share tree fixed
- Save ID3 tags
- Removed bug with floating bars (no commend messages received...)
- Help files added

Version 0.7
====================
- Many bug-fixes :-)
- Possibility to add shares and set the download directory
- Create new accounts
- Browser navigation
  To i.e. search for a CD cover, click on a file in the MP3 list and hit the
  "Search CD covers" in the toolbar
- Transfer meter that works :-)
- Better WinAmp support. If you hit play when a file is selected, the file is queued
  in the WinAmp playlist. If you doubleclick on a file, it is played immediately.

CONTACT
======================================================================================
Author:   Jean-Pierre Bergamin <james@ractive.ch>
Homepage: http://www.ractive.ch/gpl/AGStreme.html
E-mail:   AGStreme@ractive.ch
======================================================================================