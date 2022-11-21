#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

size_t calculateInternalFrag(size_t size)
{
    size_t totalSize = size + sizeof(MallocHeader);
    if (totalSize >= 2048)
        return 0;
    size_t order = ceil(log2(totalSize));
    size_t frag = pow(2, order) - (totalSize);
    return frag;
}

size_t calculateInternalFrag1(size_t size,size_t alignment)
{
    size_t totalSize = size + 2*sizeof(MallocHeader)+alignment;
    if (totalSize >= 2048)
        return 0;
    size_t order = ceil(log2(totalSize));
    size_t frag = pow(2, order) - (size + 2*sizeof(MallocHeader));
    return frag;
}

void menu_options()
{
    cout << "Choices: " << endl;
    cout << "1. malloc " << endl;
    cout << "2. calloc" << endl;
    cout << "3. realloc " << endl;
    cout << "4. realloc_array" << endl;
    cout << "5. memalign " << endl;
    cout << "6. posixmemalign " << endl;
    cout << "7. free " << endl;
    cout << "8. exit " << endl;
}

void menu()
{
    int choice;
    int *mem;
    while (1)
    {
        menu_options();
        cout << "Enter Option: ";
        cin >> choice;
        size_t size;

        //MAlloc
        if (choice == 1)
        {
            cout << "Enter the size: ";
            cin >> size;
            mem = (int *)my_malloc(size);
            
            printf("Successfully malloc'd %zu bytes at start addr %p and at ending addr %p\n", size, mem,(char*)mem+(size-1));
            
            cout<<"Internal fragmention of: "<<calculateInternalFrag(size)<<" byte"<<endl;
        }
        //calloc
        else if (choice == 2)
        {
            size_t member;
            cout << "Enter the number of members: ";
            cin >> member;
            cout << "Enter size of one member: ";
            cin >> size;
            mem = (int *)my_calloc(member, size);
            printf("Successfully calloc'd %zu bytes at start addr %p and at ending addr %p\n", member*size, mem,(char*)mem+(member*(size) -1));
            
            cout<<"Internal fragmention of: "<<calculateInternalFrag(size*member)<<" byte"<<endl;
        }

        //realloc
        else if (choice == 3)
        {
            cout << "Enter size of to reallocate: ";
            cin >> size;
            mem = (int *)my_realloc(mem, size);
            printf("Successfully realloc'd %zu bytes at start addr %p and at ending addr %p\n", size, mem,(char*)mem+(size-1));
            
            cout<<"Internal fragmention of: "<<calculateInternalFrag(size)<<" byte"<<endl;
        }

        ///realloc_array
        else if (choice == 4)
        {
            size_t member;
            cout << "Enter the number of members: ";
            cin >> member;
            cout << "Enter size of one member: ";
            cin >> size;
            mem = (int *)my_reallocarray(mem, member, size);
            printf("Successfully malloc'd %zu bytes at start addr %p and at ending addr %p\n", size*member, mem,(char*)mem+(member*size-1));
            
            cout<<"Internal fragmention of: "<<calculateInternalFrag(size*member)<<" byte"<<endl;
        }

        //Memalign
        else if (choice == 5)
        {
            size_t alignment;

            cout << "Enter the byte alignment (Alignment should be in power of 2): ";
            cin >> alignment;
            cout << "Enter the size: ";
            cin >> size;
            mem = (int *)my_memalign(alignment, size);
            printf("Successfully memalign'd %zu bytes at start addr %p and at ending addr %p\n", size, mem,(char*)mem+(size-1));
            
            cout<<"Internal fragmention of: "<<calculateInternalFrag1(size,alignment)<<" byte"<<endl;
        }

        //posixmemalign
        else if (choice == 6)
        {
            void *memptr=NULL;
            size_t alignment;
            cout << "Enter the byte alignment (Alignment should be in power of 2): ";
            cin >> alignment;
            cout << "Enter the size: ";
            cin >> size;
            my_posix_memalign(&memptr, alignment, size);
            printf("Successfully posiz_memalign'd %zu bytes at start addr %p and at ending addr %p\n", size, memptr,(char*)memptr+(size-1));
            
            cout<<"Internal fragmention of: "<<calculateInternalFrag1(size,alignment)<<" byte"<<endl;
        }
        else if (choice == 7)
        {
            my_free((void *)mem);
            printf("Successfully free'd %zu bytes from addr %p\n", size, mem);
        }
        else if (choice == 8)
        {
            break;
        }
        else
        {
            cout << "Invalid Command!!" << endl;
        }
    }
}



int main(int argc, char **argv)
{
    menu();
    return 0;
}