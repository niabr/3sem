/*
Разработайте программу-демон, которая будет следить за содержимым/состоянием некоторой директории на файловой системе.
Варианты программы (выберите любой):

1) subrevision-daemon: демон, отслеживающий рекурсивно изменения любых файлов и директорий внутри нужной директории, и записывающий их в некоторый лог-файл. Например, если между двумя итерациями считывания содержимого данные в файле 1.txt изменились, вывести разность между ними в git-like виде (+ данные /-данные) или в diff-like (> ревизия n-1 / < ревизия n). Хранить и выводить можно только поледнюю ревизию.
 
2) inotify-daemon: демон, отслеживающий обращения к содержимому нужной директории через интерфейс inotify-подсистемы ядра Linux: получите inotify-файловый дескриптор и отслеживайте системные вызовы. Демон должен выводить данные в лог в формате время: вызов(некоторая информация: параметры, данные и т.д)

Обязательное условие: демон (на то он и демон) должен быть отвязан от любого терминала, устойчив к посылке сигналов, кроме явных SIGKILL и SIGSTOP, сохранять свой PID, не реагировать на нажатия комбинаций клавиш.
*/
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<limits.h>
#include<stdlib.h>

int main(){
    char dir[4097];
    getwd(dir);
    int fd=open("diff.log",O_RDWR|O_CREAT|O_TRUNC,0666);
    pid_t pid=fork();
    if(pid){
	int fd2=open("dpid",O_RDWR|O_CREAT|O_TRUNC,0666);
	write(fd2,&pid,sizeof(pid_t));
	close(fd2);
	close(fd);
	exit(0);
    }
    else{
	setsid();
	chdir("/");
	//umask(0);
	close(0);
	close(1);
	close(2);

	dup2(fd,1);
	int dd=mkdir("/tmp/temp",0777);
	pid=fork();
	if(pid==0){
	    execl("/bin/cp","cp","-r",dir,"/tmp/temp/dir",0);
	}
	wait();
	while(1){
	    pid=fork();
	    lseek(fd,0,SEEK_SET);
	    if(pid==0){
		execl("/bin/diff","diff","-r",dir,"/tmp/temp/dir",0);
		//execl("/bin/echo","echo","hello world");
	    }
	    wait();
	    sleep(10);
	}
    }
    return 0;
}
