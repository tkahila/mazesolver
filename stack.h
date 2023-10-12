
//Point structure
typedef struct {
    int x;
    int y;
    char directionMark;
} Point;

//Stack strucutre
typedef struct {
    Point *data; 
    int top;    
    int capacity; 
} Stack;

// Functions
Stack *createStack(int capacity);
int isEmpty(Stack *stack);
int isFull(Stack *stack);
void push(Stack *stack, Point point);
Point pop(Stack *stack);
Point peek(Stack *stack);
void destroyStack(Stack *stack);

