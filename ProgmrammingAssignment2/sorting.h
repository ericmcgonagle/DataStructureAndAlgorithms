#pragma once

#include <functional> // std::less
#include <iterator>	  // std::iterator_traits

namespace sort
{

	// This is C++ magic which will allows our function
	// to default to using a < b if the comparator arg
	// is unspecified. It uses defines std::less<T>
	// for the iterator's value_type.
	//
	// For example: if you have a vector<float>, the
	// iterator's value type will be float. std::less
	// will select the < for sorting floats as the
	// default comparator.
	template <typename RandomIter>
	using less_for_iter = std::less<typename std::iterator_traits<RandomIter>::value_type>;

	/* Efficiently swap two items - use this to implement your sorts */
	template <typename T>
	void swap(T &a, T &b) noexcept
	{
		T temp;
		temp = std::move(a);
		a = std::move(b);
		b = std::move(temp);
	}

	template <typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void bubble(RandomIter begin, RandomIter end, Comparator comp = Comparator{})
	{
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings
		using _it = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		using value_type = typename _it::value_type;
		using reference = typename _it::reference;
		using pointer = typename _it::pointer;

		// TODO
		difference_type size = end - begin;
		// std::cout << "ELEMENTS IN THE VECTOR: " << std::endl;
		for (difference_type i = 0; i < size; ++i)
		{
			// std::cout << *(i + begin) << std::endl;
		}
		for (difference_type i = 1; i < size; ++i)
		{
			bool cont = false;
			for (difference_type j = 0; j < size - i; ++j)
			{
				// value_type curr = *(j + begin);
				// value_type next = *(j + 1 + begin);
				// std::cout << "Comparing: " << *(j + begin) << " with " << *(j + 1 + begin) << std::endl;
				if (comp(*(j + 1 + begin), *(j + begin)))
				{
					swap(*(j + begin), *(j + 1 + begin));
					// std::cout << "SWAPPING: " << *(j + begin) << " with " << *(j + 1 + begin) << std::endl;
					cont = true;
				}
			}
			if (!cont)
				break;
		}
	}

	template <typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void insertion(RandomIter begin, RandomIter end, Comparator comp = Comparator{})
	{
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings
		using _it = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		using value_type = typename _it::value_type;
		using reference = typename _it::reference;
		using pointer = typename _it::pointer;

		difference_type size = end - begin;

		for (difference_type i = 1; i < size; ++i)
		{
			// difference_type temp = i;
			//difference_type index = i;
			//difference_type k = i - 1;
			difference_type index = i;

			for (difference_type k = i - 1; k >= 0 && comp(*(index + begin), *(k + begin)); --k) {
				swap(*(index + begin), *(k + begin));
				index--;
			}



			// while (k >= 0 && (comp(tmp, *(k + begin))))
			// {
			// 	std::cout << *(i + begin) << " IS LESS THAN: " << *(k + begin) << std::endl;
			// 	swap(tmp, *(k + begin));
			// 	k--;
			// 	//i--;
			// }

			//*(index + begin) = std::move(tmp);

			// if (*(k + 1 + begin) != *(i + begin))
			// 	swap(*(k + 1 + begin), *(i + begin));
		}
	}

	template <typename RandomIter, typename Comparator = less_for_iter<RandomIter>>
	void selection(RandomIter begin, RandomIter end, Comparator comp = Comparator{})
	{
		// Random access iterators have the same traits you defined in the Vector class
		// For instance, difference_type represents an iterator difference
		// You may delete the types you don't use to remove the compiler warnings
		using _it = std::iterator_traits<RandomIter>;
		using difference_type = typename _it::difference_type;
		using value_type = typename _it::value_type;
		using reference = typename _it::reference;
		using pointer = typename _it::pointer;

		difference_type size = end - begin;

		for (difference_type i = 0; i < size; ++i)
		{
			difference_type index = i;
			for (difference_type k = i + 1; k < size; ++k)
			{
				if (comp(*(k + begin), *(index + begin)))
				{
					index = k;
				}
			}
			swap(*(i + begin), *(index + begin));
		}
	}
}
