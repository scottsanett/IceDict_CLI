#Old Icelandic Dictionary

#Info:
This application contains two Old Icelandic English dictionaries from Zoega and Vifgusson, respectively.
It supports searching searching Icelandic words for its definition and searching text the said dictionaries with or without regular expression. Regex is implemented with the C++ regex library, and searches may be accomplished with ECMAscript regex syntax.

This application also supports inflectional search. Source data of this search is provided by bin.arnastofnun.is/forsida, where one may download a copy of the csv data file.
One may insert any word in the database (indicated by the alternatives displayed below as one type) to find out its original form as well as its inflectional status. 
One may also choose to print all inflectional forms of a word, in which case the word needs to be the originial form. 
Lastly, one may find a specific inflectional form of a word. The inflectional forms (from where one may choose) are displayed below as one type.

#Build:
This dictionary runs on LINUX only.

Dependencies: ncurses, pthread

The dictionary files can be downloaded from this link: 
http://pan.baidu.com/s/1qXDZ7Ak

Compile:

1. Create a new folder named alphabet in the root directory of the dictionary and extract all files in the rar file into ./alphabet.

2. Run ./configure (The uploaded configure file works for x64 Linux. One needs to modify the configure file for other architectures).

3. Run ./dic and you're good to go.

