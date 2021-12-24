#include <iostream>

int main(int argc, char* argv[]) {


    if (argc < 2){
        std::cout << "You forget to pass the N value!!!";
        return 0;
    }

    const int N = atoi(argv[1]);

    if(N <= 0){
        std::cout << "You wrote the wrong argument!!!" << std::endl << "It must be a number grater than 0";
        return 0;
    }

   std::cout<< "All right N=" << N << std::endl;




    return 0;
}
