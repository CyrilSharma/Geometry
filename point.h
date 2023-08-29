#pragma once
template <typename T>
struct point {
    T x, y;
    bool operator<(const point &other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    bool operator>(const point &other) const {
        return other < *this;
    }
    bool operator==(const point &other) const {
        return (x == other.x && y == other.y);
    }
    bool operator<=(const point &other) const {
        if (x != other.x) return x < other.x;
        return y <= other.y;
    }
    bool operator>=(const point &other) const {
        if (x != other.x) return x > other.x;
        return y >= other.y;
    }
};