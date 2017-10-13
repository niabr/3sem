/* Рассчитать определённый интеграл какой-нибудь простой, интегрируемой аналитически на некотором промежутке функции f(x) методом Монте-Карло:
расчёт проводить параллельно в n-потоков некоторой программы А, генерируя в каждом из них N/n точек равномерно как по интервалу, так и по области значений.
Собирать и обрабатывать результаты в программе Б, читающей результаты из разделяемой памяти. [В случае использования независимых ячеек разделяемой памяти, 
сохранение результатов можно обеспечить без синхронизации, в случае разделяемой ячейки в разделяемой памяти - синхронизация необходима. Реализация способа хранения
влияет на результаты эксперимента 2 а,б*) (см. ниже).

Задачи, поставленные перед Вами как исследователем:
1) Оценить прирост производительности в связи с использованием n' потоков вместо 1, где n' - число физических ядер Вашей машины. 
Узнать число ядер-например, 'cat /cpu/procinfo'
2) Провести серию измерений, в которой, увеличивая число n' :
  а) пронаблюдать стагнацию роста производительности при увеличении числа потоков
  б*) определить, при каких n' (насколько больших) накладные расходы от использования многопоточности [и синхронизации, если она имела место]
  превосходят преимущества их использования.
*/

#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<time.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/shm.h>
#include<pthread.h>
#include<math.h>
#define N 100000000

int n;

double f(double x){
    double y = sin(x);
    return y;
}

double F(double x){
    double y = -cos(x)+1;
    return y;
}


void *thread(void *arg){
    int i;
    double x;
    double y;
    for(i=0;i<=N/n;i++){
	x=(double)rand()/RAND_MAX;
	y=(double)rand()/RAND_MAX;
	if(y<=f(x))
	    ( *((int*)arg))++;
	}
    printf("%d\n",i);
    return NULL;
}

int main(){
    int i;
    scanf("%d",&n);
    int fd = shmget(IPC_PRIVATE,n*sizeof(int), 0666);
    int *p = shmat(fd,NULL,0);
    srand(time(NULL));
    pthread_t thid;
    for(i=0;i<n;i++){
	pthread_create(&thid,(pthread_attr_t *)NULL,thread,p+i);
    }
    pthread_join(thid, (void **)NULL);
    int sum=0;
    for(i=0;i<n;i++) sum+=p[i];
    double ans = (double)sum/N;
    printf("Divergence:%lf\n",ans-F(1));
    
    shmdt(p);
    return 0;
}
