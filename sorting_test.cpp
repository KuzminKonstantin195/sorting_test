#include <vector>
#include <chrono>
#include <iostream>

#include <thread>

using namespace std;

/*################################################################################################################################
*   Тестирование основных алгоритмов сортировки (для иллюстрации принципа Big "O" notation):
*       - пузырьковая (для референса);
*       - вставками;
*       - быстрая, ak сортировка Хоара;
*       - сортировка слиянием;
*   Условия: всё делается на одном потоке
*
*   Проведены тесты обработки массива длинной 100, 1000 и 100 000 элементов Результаты:
*                                   100         1000        100000
*       - пузырьковая:           8.1e-05      0.00145      21.3488
*       - вставками:             3.3e-05      0.000808     13.0965
*       - быстрая :              1.7e-05      6.8e-05      0.017959
*       - сортировка слиянием:   0.000147     0.000511     0.142963
*   
*
################################################################################################################################*/
const int arr_sz = 100;

int arr [arr_sz];
int _arr [arr_sz];

void arr_init (int* arr, const int size)
{
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();

    srand(seed);
    for (auto i = 0; i < size; i++)
    { 
        arr[i] = rand() % (arr_sz*10);
    }
}

void reb_arr (int* const_arr,  int* rev_arr)
{
    

    for (auto i = 0; i < arr_sz; i++)
    {
        rev_arr[i] = const_arr[i];
    }
    cout << "Rebuild complete!" << endl;

}

// первое - сортировка пузырьком
void bubble_sort (int* start_arr, int len)
{
    for (int inter_count = len - 1; inter_count > 0; inter_count--)
    {
        for (int pos = 0; pos < inter_count; pos ++)
        {
            if (start_arr[pos] > start_arr[pos+1])
            {
                swap(start_arr[pos], start_arr[pos+1]);
            }
        }
    }
    
}       

// второе - сортировка вставками
void insert_sort (int* start_arr, int len)
{
    for (int pos = 1; pos < len; pos++)
    {
        int loc_pos = pos;
        int prev_pos = loc_pos-1;
        
        while ((start_arr[loc_pos] < start_arr[prev_pos]) && loc_pos>0)
        {
            swap(start_arr[loc_pos], start_arr[prev_pos]);
            --loc_pos;
            prev_pos = loc_pos-1;
        }
    }
}

// быстрая сортировка (start - включительно, end - не включает)
void quick_sort (int* start_arr, int start, int end)
{
    if (end-start < 2)
    {
        return;
    }

    int left {start}, right{end};

    int pivot_pos = (start+end)/2;
    int pivot = start_arr[pivot_pos];

    while (left <= right)
    {
        while (start_arr[left] < pivot)
        {
            left++;
        }
        while (start_arr[right] > pivot)
        {
            right--;
        }

        if (left <= right)
        {
            swap (start_arr[right], start_arr[left]);
            left++;
            right--;
        }
    }

    quick_sort(start_arr, start, left-1);
    quick_sort(start_arr, left, end);

    return;
}

// сортировка слиянием
void merge_sort (int* arr, int start, int end)
{
    if (start == end)
    {
        return;
    }
    
    
    int mid_pos = (end+start)/2;
    
    merge_sort(arr, start, mid_pos);
    merge_sort(arr, mid_pos+1, end);
    
    int left = start; 
    int right = mid_pos+1;
    
    int sz = end-start+1;
    int* buffer = new int [sz] {0};
    
    for(int i = 0; i < sz; i++)
    {
        if (left <= mid_pos && right <= end)
        {
            if (arr[left] <= arr[right])
            {
                buffer[i] = arr[left];
                left++;
            } else
            {
                buffer[i] = arr[right];
                right++;
            }
        } 
        else 
        {
            if (left > mid_pos)
            {
            buffer[i] = arr[right];
            right++;
            } else
            {
            buffer[i] = arr[left];
            left++;
            }
        }
    }
    
    for (int i = 0; i < sz; i++)
    {
        arr[start +i] = buffer[i];
    }
    
    delete buffer;
    return;
}


void result_out ()
{
    cout << "Result: \n";
    for (auto i = 0; i < arr_sz; i++)
    {
        cout << _arr[i] << ", ";
    }
    cout << endl;
}

int main ()
{
    cout << "Start!" << endl;
    cout << "Array lenght: " << arr_sz << endl;

    arr_init(arr, arr_sz);
    reb_arr (arr, _arr);
    //result_out ();
    auto start_point = chrono::high_resolution_clock::now ();
        bubble_sort(_arr, arr_sz);
    auto end_point = chrono::high_resolution_clock::now ();
    chrono::duration<float> dur = end_point - start_point;
    //result_out ();
    cout << "Bubble sort duration: " << dur.count() << endl;

    
    reb_arr (arr, _arr);
    start_point = chrono::high_resolution_clock::now ();
        insert_sort(_arr, arr_sz);
    end_point = chrono::high_resolution_clock::now ();
    dur = end_point - start_point;
    //result_out ();
    cout << "Inserting sort duration: " << dur.count() << endl;

    reb_arr (arr, _arr);
    start_point = chrono::high_resolution_clock::now ();
        quick_sort(_arr, 0, arr_sz-1);
    end_point = chrono::high_resolution_clock::now ();
    dur = end_point - start_point;
    //result_out ();
    cout << "Quick sort duration: " << dur.count() << endl;

    reb_arr (arr, _arr);
    start_point = chrono::high_resolution_clock::now ();
        merge_sort(_arr, 0, arr_sz-1);
    end_point = chrono::high_resolution_clock::now ();
    dur = end_point - start_point;
    //result_out ();
    cout << "Merge sort duration: " << dur.count() << endl;


    cout << "End!";
    return 0;
}