# Antlr-C-

Progetto per la costruzione e visita di un albero sintattico a partire dal codice C,
ai fini dell'interpretazione passo-passo del programma, con finalita' di debugging.

# how to run

fare clone dalla repository.

Inserire il codice C che si vuole testare nel file di nome "input.c" all'interno della cartella Antlr-C- oppure passando il path al file come 
parametro dell'eseguibile.

lanciare l'eseguibile da terminale nella cartella Antlr-C-
* ./m

Per far partire il progetto è necessario installare la libreria runtime di c libantlr3c-3.1.1 compresa in questa repository git, per farlo è necessario
entrare nella cartella libantlr3c-3.1.1 da terminale e lanciare i comandi:


* ./configure --enable-64bit


* make


* sudo make install

https://www.antlr3.org/api/C/build.html per ulteriori dettagli.

se si vuole modificare la grammatica o un file .c quale il main e quindi aggiornare l'eseguibile:

# *how to modify executable*

(passaggi necessari per creare un nuovo eseguibile dopo aver modificato la grammaticao un file .c)

Il debugger utilizza la libreria glib per le hashtable, la libreria si può installare con il comando

* sudo apt-get install libgtk2.0-dev

In seguito, per generare il codice che antlr utilizza per riconoscere le grammatiche e generare gli alberi, spostarsi all'interno della cartella
Antlr-C- della repository e lanciare il comando:

* java -jar ../antlr-3.5.2-complete.jar SimpleCWalker.g SimpleC.g

è normale la quantità di warning per le differenze della libreria runtime C e Java.

infine per compilare il codice .c e creare l'eseguibile, usare il comando:


* gcc main.c SimpleCLexer.c SimpleCParser.c SimpleCWalker.c instr_evaluator.c -g -o m -I/usr/local/include -L /usr/local/lib -l antlr3c `pkg-config --cflags --libs glib-2.0`


infine lanciare l'eseguibile 


* ./m

