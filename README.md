# CourseProject-SocketProgramming
This is the socket programming use C and implemented with TCP and UDP socket programming

Assignment Achievement: In this assignment, I successfully established a connection between a client and a server and transfer message using TCP. What’s more, in the server it also established three connections with another three server and transfer message using UDP. When client got two messages from command input window, it would combine them into one message and sent it to server named ‘aws’. In server aws it would separate it into two message which was function and value. After that server aws would sent value to three backend servers to get different results of value. serverA would return the square of value it received; serverB would return the cube of value it received; serverC would return the 5th power of value it received. Aws will collect the square, cube, 4th, 5th, 6th power of value. After that, aws will calculate the final result based on the function it got from the message. If the function is ‘LOG’, then it would calculate − x – x^2/2 – x^3/3 – x^4/4 – x^5/5 – x^6/6. If the function is ‘DIV’, it would calculate 1+ x^2 +x^3 + x^4 +x^5 + x^6. Then aws would send the final result to the client and client would print the result on command window. I didn’t use fork() function in this project, so we need 5 command window to run client, aws, serverA, serverB, serverC separately. And the way I calculate the numbers are follow the power order, that means I send the value to server A to get x^2, then send value to server B to get x^3, send x^2 to server A to get x^4 and so on. Therefore, the output message order is the same as the orders in aws server command window.

d.	Code files and descriptions:

Client.c: this file initiates a connection between client and server aws using TCP, and combines the function and value from input and send them as one message. Once it gets the value from command window, it will send the message to aws through port <25584>, also it will wait to receive result from server aws and display it on screen.

Aws.c: this file will boot up as TCP server, waiting for message from TCP client from the port <25584>, once it gets the message, it will divide them into function and value, then establishes connection to three backend servers using UDP. After getting results from three backend server (over UDP port <24584>) it will calculate the final result based on function it gets from client. Then it will send the final result to client using TCP.

serverA.c: this file will initiate as UDP server and listen to the message from aws over port <21584>, and it will return the power value to aws.

serverB.c: this file will initiate as UDP server and listen to the message from aws over port <22584> from aws, and it will return the cube value to aws.

serverC.c: this file will initiate as UDP server and listen to the message from aws over port <23584> from aws, and it will return the 5th value to aws.

e.	The format of all the messages exchanged.

The format of all messages exchanged is string, I used a char array to store all the messages between client and aws, aws to all backend servers. In the client, when it gets input from command window, it will combine the <function> and <value> into one string, and send it to aws. In aws, it will divide them into two parts, one is <function> and the other is <value>. Then it will send <value> as string format to all backend servers. Backend servers are able to convert string to float, calculate the float and convert it to string and send it back to aws. Therefore, all the format of messages exchanged are all strings.


f.	I reused code from Beej’s tutorial guide, I reused the block of initiation of socket addresses and the clean-up of socket address memory, it’s already been stated inside all the source code in the comment. During debugging, sometimes I encountered that “port has already been used”, that is because it takes some time to release the port. To resolve this problem, I used function “setsockopt”. This idea is from stackoverflow.com. and has already stated in source code.
