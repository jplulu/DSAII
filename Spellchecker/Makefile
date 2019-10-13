spell.exe: spellcheck.o hashTable.o
	g++ -o spell.exe spellcheck.o hashTable.o

spellcheck.o: spellcheck.cpp hashTable.h
	g++ -c spellcheck.cpp

hashTable.o: hashTable.cpp hashTable.h
	g++ -c hashTable.cpp

debug:
	g++ -g -o spellDebug.exe spellcheck.cpp hashTable.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
