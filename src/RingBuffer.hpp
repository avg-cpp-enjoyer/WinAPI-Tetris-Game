#pragma once

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>

template<typename T, std::size_t n, typename = std::enable_if<(n & (n - 1)) == 0, void>>
class RingBuffer {
public:
	using value_type       = T;
	using size_type        = std::size_t;
	using reference        = T&;
	using const_reference  = const T&;

	RingBuffer() noexcept = default;

	bool push(const T& value) noexcept {
		size_type next = (m_tail + 1) & m_mask;
		if (next == m_head) return false;
		m_data[m_tail] = value;
		m_tail = next;
		return true;
	}

	bool push(T&& value) noexcept {
		size_type next = (m_tail + 1) & m_mask;
		if (next == m_head) return false;
		m_data[m_tail] = std::move(value);
		m_tail = next;
		return true;
	}

	bool pop(T& out) noexcept {
		if (empty()) return false;
		out = std::move(m_data[m_head]);
		m_head = (m_head + 1) & m_mask;
		return true;
	}

	const_reference front() const {
		if (empty()) throw std::runtime_error("RingBuffer is empty");
		return m_data[m_head];
	}

	reference front() {
		if (empty()) throw std::runtime_error("RingBuffer is empty");
		return m_data[m_head];
	}

	void clear() noexcept {
		m_head = m_tail = 0;
	}

	bool empty() const noexcept {
		return m_head == m_tail;
	}

	bool full() const noexcept {
		return ((m_tail + 1) & m_mask) == m_head;
	}

	size_type size() const noexcept {
		return (m_tail + n - m_head) & m_mask;
	}

	static constexpr size_type capacity() noexcept {
		return n - 1;
	}

	class iterator {
	public:
		using iterator_category  = std::forward_iterator_tag;
		using value_type         = T;
		using difference_type    = std::ptrdiff_t;
		using pointer            = T*;
		using reference          = T&;

		iterator(RingBuffer* buf, size_type pos) noexcept : m_buf(buf), m_pos(pos) {}

		reference operator*() const noexcept {
			return m_buf->m_data[m_pos];
		}
		pointer operator->() const noexcept {
			return &m_buf->m_data[m_pos];
		}

		iterator& operator++() noexcept {
			m_pos = (m_pos + 1) & m_buf->m_mask;
			if (m_pos == m_buf->m_tail) m_pos = m_buf->m_tail;
			return *this;
		}
		iterator operator++(int) noexcept {
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		friend bool operator==(const iterator& a, const iterator& b) noexcept {
			return a.m_buf == b.m_buf && a.m_pos == b.m_pos;
		}
		friend bool operator!=(const iterator& a, const iterator& b) noexcept {
			return !(a == b);
		}

	private:
		RingBuffer* m_buf;
		size_type   m_pos;
	};

	class const_iterator {
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = const T*;
		using reference = const T&;

		const_iterator(const RingBuffer* buf, size_type pos) noexcept : m_buf(buf), m_pos(pos) {}

		reference operator*() const noexcept {
			return m_buf->m_data[m_pos];
		}
		pointer operator->() const noexcept {
			return &m_buf->m_data[m_pos];
		}

		const_iterator& operator++() noexcept {
			m_pos = (m_pos + 1) & m_buf->m_mask;
			if (m_pos == m_buf->m_tail) m_pos = m_buf->m_tail;
			return *this;
		}
		const_iterator operator++(int) noexcept {
			const_iterator tmp = *this;
			++*this;
			return tmp;
		}

		friend bool operator==(const const_iterator& a, const const_iterator& b) noexcept {
			return a.m_buf == b.m_buf && a.m_pos == b.m_pos;
		}
		friend bool operator!=(const const_iterator& a, const const_iterator& b) noexcept {
			return !(a == b);
		}

	private:
		const RingBuffer* m_buf;
		size_type         m_pos;
	};

	iterator begin() noexcept {
		return iterator(this, m_head);
	}
	iterator end() noexcept {
		return iterator(this, m_tail);
	}
	const_iterator begin() const noexcept {
		return const_iterator(this, m_head);
	}
	const_iterator end() const noexcept {
		return const_iterator(this, m_tail);
	}
	const_iterator cbegin() const noexcept {
		return begin();
	}
	const_iterator cend() const noexcept {
		return end();
	}

private:
	static constexpr size_type m_mask = n - 1;
	T m_data[n];
	size_type m_head = 0;
	size_type m_tail = 0;
};