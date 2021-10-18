# Group_P4_Build_It
Group project 4: Build It for CMSC414, Group members: Basem A, Prad B, and Mohammad Subhan

# Project ask
Students in teams of up to 3 students will collaborate to write a 
protocol for communication among atm and a bank . There will be two 
programs. One program, called atm , will allow bank customers to 
withdraw and deposit money from their account. The other program, 
called bank , will run as a server that keeps track of customer balances

# Project description
![protocol](https://user-images.githubusercontent.com/32045565/137764445-981533ef-62d9-47b0-ac77-aaeedc182861.png)

## Overview
Everything starts when the Bank server is created. To start the Bank server, you can simply call
it with “./bank” alone or you can specify the port and auth file to be used, both of which are error
checked using regex. The Bank writes a file called bank.auth, fills it with a salted SHA256 hash
and signs it using OpenSSL. The bank.auth is then “shared” between the Bank and ATM to
verify that the Bank is talking to the correct ATM and vice versa. If only the names match but
the contents do not, the Bank will send a 255 error to the ATM. The bank.auth cannot be
replicated or acquired by attackers according to the project documentation. Once bank.auth is
created, the server will store its name (if specified to) and print created to the console. This
prompts the ATM that the server is now ready to process commands. The server loops until it
receives SIGTERM, upon which it frees all data structures and deletes the auth file used.
On the ATM side, it processes commands one at a time, using argz.h to verify that all
required options (account name and command) are provided. Any parameters passed to the ATM
are then checked using regex and if they are valid, they are executed. If an account is new, a card
file is created by the Bank, but the account name must match whatever was first passed with the
account upon creation. Account names are unique and cannot be used more than once for
different accounts. Once all commands given to the ATM are processed, it creates the command
to send to the Bank in the format [command] [account name] [account card] [amount to
deposit/withdraw] [auth file used], then encrypts it using a randomly generated IV and bank.auth
as the key. If bank.auth is not the same bank.auth that was created by the server, decryption will
fail and the server will return invalid. This ensures that the server is not modified by an
unrecognized ATM. For encryption and decryption, the ATM sends an IV to the Bank and
attaches the encoded message. The Bank receives both and uses the IV which is the first 16
characters to decrypt the rest of the message. The message is split by spaces and each of the
ATM parameters sent are checked for validity. If the command is valid, then the Bank alters the
account and stores it into the list data structure. A message in JSON format is then encrypted by
the Bank using bank.auth as the key and sent back to the ATM which in turn either exits if
invalid or prints the JSON response if valid.

## Attacks and Countermeasures
1. An attacker cannot pretend to be a working ATM since they do not have access to the
auth file and cannot sign nor be verified by the bank.
2. An attacker could access the packets of the encrypted message and attempt to change the
message to break the process. To successfully do this, they would need to take the first 16
characters as the IV and test the rest of the command until they find where the dollar
amounts are hidden, most likely after guessing and checking for the positions indicating
spaces as they would be the same character. However, as the key for the decryption is
stored in the auth file, it would be impossible to replicate the decryption or guess a
sequence of characters to represent a different dollar amount.
3. An attacker with a valid atm and account cannot attempt to withdraw more than they
have and they cannot deposit a large amount which would overflow the integer. Only the
intended amount changes can occur.
4. An attacker cannot repeatedly try the same command in an attempt to decrypt the
message sent back and forth. The iv in each encryption is random and the key will always
be hidden in the inaccessible auth file. Therefore, the message will have no traceable
pattern.

## Threats that we could not address
1. The largest threat that we could not address is attackers ability to obtain the .card file
used. The ATM will accept any commands for a certain account as long as that card file
matches what was used when the account was created and the bank.auth is valid (ATM
sending the command is valid). So if an attacker could steal a user’s .card file and send
commands through a valid ATM, they can steal all of a user’s money. We believe that the
best way to prevent this is to have users set up a pin when they create their account that is
linked to the card file, but we could not change the parameters without failing the tests, so
this was not implemented.
2. An attacker could delete parts of the encrypted method sent over if they somehow
accessed the packets as they were sent to the bank. However, this is not important as the
attacker would not be able to access an account and change an amount this way, or
decrypt the message.

## Contributions
### Basem Al-Rifai
- Created the overall protocol between the ATM and Bank.
- Wrote bank.c and bank-main.c to accept secure clients using salted hash bank.auth files.
- Implemented regex for Bank as well as using argz.h to ensure that arguments passed to
both ATM and Bank are POSIX compliant.
- Worked on encryption and decryption of messages between ATM and Bank using
OpenSSL.

### Pradyumn Bangera
- Co-Wrote atm.c to only accept valid commands from users using regex.
- Verified the signature of bank.auth using OpenSSL.
- Worked on encryption and decryption of messages between ATM and Bank using
- Parsed the bank.auth file to be used as a key for encrypting the message sent to the bank.

### Mohammad Subhan
- Co-Wrote atm.c to only accept valid commands from users using regex.
- Worked on documentation for the project, listing the attacks and countermeasures we
used for our protocol.
- Wrote regex for commands passed to the ATM and ensured that the ATM was POSIX
compliant.
