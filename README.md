# Operating-System
Operating System Project

========================================
<h2>Shell Interface</h2>

<p> •Created a shell interface which can execute bash command using execvp() function.</p>

<p> •Handled bash command ending with ”&” via creating a new process with fork() function without blocking current shell.</p>

<p> •From the following picture we can find "sleep 5m" process running in a different process using "ps -af" command.</p>
<img src="https://raw.githubusercontent.com/CarollChen/OS/master/Shell%20Interface/ScreenShot.jpg" />
========================================
<h2>Multi-Thread Soduko Verfication </h2>
<p> •Create 27 threads to check each row, column and block of a Soduko</p>
<img src="https://raw.githubusercontent.com/CarollChen/OS/master/Sudoku/screenShot.jpg" />
========================================
<h2>Sleeping TA</h2>
<p> •Created one TA and four student threads with Pthread.</p>

<p> •Student threads will alternate between programming for a period of time using rand r() and sleep().</p>

<p> •Used semaphore to synchronize and simulate students that are queued up and wait for TA.</p>

<p> •Lock the mutex lock on TA when the TA is busy.</p>

<p> •Utilized condition broadcast when TA is able to help next student.</p>

<img src="https://raw.githubusercontent.com/CarollChen/OS/master/Sleeping%20TA/screenShot.jpg"/>
