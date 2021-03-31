# hamming UDP transmitter

## summery
there are three seperate executables
sender.exe: reads file and sends it via UDP port to the channel
channel.exe: recieves message, adds random noise and passes it over to the reciver.exe
reciever.exe: recieves bit filped message, decodes and corrects and sends an acknolagment message back

to be absolutly honest I've probably put in writing this code wich I completely wrote from scratch over 100 hours
in the past 3 weeks and I'm done, I know it can work and I know how to do it but I've got other responsibilties and I just cant spend all my time on this.

the program is built using the Cmake tools, I used vscode but it should compile on visual studio

## Hamming
### constants
UNCODED_MSG_LEN and ENCODED_MSG_LEN need to be multiples of 11*15 
where so that UNCODED_MSG_LEN/n =11 and ENCODED_MSG_LEN/n =15 (660,990)
### functions
#### read_msg 
reads from FILE* fp in UNCODED_MSG_LEN blocks and passes 11 byte size sub-blocks to **decode**
#### decode
converts 11 bytes to an array of 88 chars representing the 88 bits in the 11 byte chunck and passes the array to **hamming_encoder**
#### hamming_encoder
creates an array of 120 encoded bits using **parity encoder**
#### parity encoder
encodes 11 bits into 15 bits using hamming code
**all these functions have a reverse decoding counterpart**
#### get_bits
get an array of bits from an array of chars (i.e bytes)
#### merge 
mergers array of bits into array of chars (i.e bytes)

## NoiseyChannel
#### class constructor
takes in a seed and n probaility adjustor and creates a uniform distribution generator from 0 to 2^16/n
#### rand_flip_bit
takes in an encoded message of length ENCODED_MSG_LEN and randomly flips a bit with a probabitly of n/2^16

## ServerUtils
### send_safe recv_safe
send and recv message to/from socket while checking for errors and printing errors to screen
#### sendto_safe recvfrom_safe
recvfrom recieves a message through a UDP socket without the need to accept a connection, saves the address and port in a sockaddr_in variable
sendto sends a message through a UDP socket using the addres and port recieved by recvfrom so that there is no need to accept a new client on a port
