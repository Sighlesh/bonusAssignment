#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void* Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n) 
{
	for (int i = n / 2 - 1; i >= 0; i--) 
	{
		heapify(arr, n, i);
	}
	for (int i = n - 1; i >= 0; i--) 
	{
		swap(&arr[0], &arr[i]);
		heapify(arr, i, 0);
	}
}

void swap(int* x, int* y) 
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

void merge(int values[], int l, int m, int r)
{

	int* temp, i, indexlength, left, mid, auxIndex;
	left = l;
	mid = m;
	auxIndex = 0;

	indexlength = r - l + 1;
	temp = (int*)Alloc(indexlength * sizeof(int));

	
	while ((left < m) || (mid <= r))
	{
		if (mid > r || (left < m && values[left] < values[mid]))
		{
			temp[auxIndex] = values[left];
			left++;
			auxIndex++;
		} else {
			temp[auxIndex] = values[mid];
			mid++;
			auxIndex++;
		}
	}
	for (i = l; i <= r; i++) {
		values[i] = temp[i - l];
	}
	DeAlloc(temp);
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r) {
		int midIndex = (l + r) / 2;
		mergeSort(pData, l, midIndex);
		mergeSort(pData, midIndex + 1, r);
		merge(pData, l, midIndex + 1, r);
	}
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{
	int i, key, j;
	for (i = 1; i < n; i++) 
	{
		key = pData[i];
		for (j = i - 1; j >= 0; j--) 
		{
			if (pData[j] > key) 
			{
				pData[j + 1] = pData[j];
			}
			else {
				break;
			}
		}
		pData[j + 1] = key;
	}
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{
	for (int step = 0; step < n - 1; step++) 
	{
		for (int i = 0; i < n - step - 1; i++) 
		{
			if (pData[i] > pData[i + 1])
			{
				int temp = pData[i];
				pData[i] = pData[i + 1];
				pData[i + 1] = temp;
			}
		}
	}
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{
	int min;
	for (int step = 0; step < n - 1; step++)
	{
		min = step;
		for (int i = step + 1; i < n; i++)
		{
			if (pData[i] < pData[min]) 
			{
				min = i;
			}
		}
		swap(&pData[min], &pData[step]);
	}
}

// parses input file to an integer array
int parseData(char* inputFileName, int** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	*ppData = NULL;
	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (int*)Alloc(sizeof(int) * dataSz);
		// Implement parse data block
		int i, n, *data;
		if (*ppData == NULL)
		{
			printf("Failed to allocate\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			fscanf(inFile, "%d ", &n);
			data = *ppData + i;
			*data = n;
		}
		fclose(inFile);
	}
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i = 0; i < 100; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\t");

	for (i = sz; i < dataSz; ++i)
	{
		printf("%d ", pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
	double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt" };

	for (i = 0; i < 3; ++i)
	{
		int* pDataSrc, * pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int*)Alloc(sizeof(int) * dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n", dataSz);
		printf("---------------------------\n");

		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}

}
