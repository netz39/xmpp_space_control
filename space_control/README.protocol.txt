XMPP-Nachrichtenprotokoll
=========================

Die aktuelle Implementierung verwendet Text-Nachrichten zur Kommunikation. Bislang gibt es außer XMPP message sessions keinen Mechanismus, um parallel laufende Nachrichten auseinander zu halten. Das kann bei Bedarf später transparent eingefügt werden.

Eine Nachricht hat immer folgendes Format:

<Command>
<N> <parameter name>
<N Zeilen parameter value>
<N> <parameter name>
<N Zeilen parameter value>
…

Beispiel:
ic2.read16
1 device
0x22
1 register
0x95


Beispiel 2:
helloworld
1 subject
hallo welt!
3 body
Hallo Welt,
ich kann auch
Zeilenumbrüche!



Die Commands werden durch die jeweiligen Applikationen festgelegt. Dabei sind bislang folgende Prefixe vergeben:

i2c.	I2C-Anbindung
