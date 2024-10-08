#pragma once
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
#include "../Log.hpp"
#include "../disableWarn.hpp"
#include "../format.hpp"
#include "../headers.hpp"
#include "Times.hpp"
#include "timeFactors.hpp"
// On GCC < 4.8, the following define is often missing. Since
// this library only uses sleep_for, this should be safe
#if defined(__GNUC__) && !defined(__clang__) && __GNUC__ < 5 && __GNUC_MINOR__ < 8
#define _GLIBCXX_USE_NANOSLEEP
#endif

namespace vnd {
    static inline constexpr auto simpleFomrat = "{}: Time = {}";
    static inline constexpr auto bigTimesFotmat = "Time = {}";
    static inline constexpr auto bigTitleTimeFotmat = "|{0: ^{1}}|{2: ^{3}}|";
    static inline constexpr auto bigFotmat = "\n{0:-^{1}}\n{2}\n{0:-^{1}}";
    static inline constexpr auto compatFotmat = "[{}]{}";
    static inline constexpr auto detaildFotmat = "Timer '{}' measured a duration of {}";
    static inline constexpr auto blockPatternFotmat = "{0:=^{1}}|{0:=^{1}}|{0:=^{1}}|{0:=^{1}}";
    static inline constexpr auto blockTimesFotmat = "Time:{}";
    static inline constexpr auto blockFotmat = "\n{0}\n{2: ^{1}}\n{0}\n{3: ^{1}}\n{0}";
    static inline constexpr auto minimalFotmat = "{} - {}";
    static inline constexpr auto timeItFotmat = "{} for {} tries";
    DISABLE_WARNINGS_PUSH(6005 26447 26455 26496)

    // NOLINTBEGIN(*-include-cleaner)
    /**
     * @brief Timer class for measuring the execution time of code.
     */
    class Timer {  // NOLINT(*-special-member-functions)
    protected:
        /// This is a typedef to make clocks easier to use
        using clock = ch::high_resolution_clock;
        /// This typedef is for points in time
        using time_point = ch::time_point<clock>;

        /// This is the type of a printing function, you can make your own
        using time_print_t = std::function<std::string(std::string, std::size_t, ValueLable)>;
        using nanolld = ch::duration<long double, std::nano>;
        /// This is the title of the timer
        std::string title_;
        std::size_t title_lenpadd;

        /// This is the function that is used to format most of the timing message
        time_print_t time_print_;

        /// This is the starting point (when the timer was created)
        time_point start_;

        /// This is the number of times cycles (print divides by this number)
        std::size_t cycles{1};

    public:
        /// Standard print function, this one is set by default
        /**
         * @brief Default print function for Timer class.
         */
        static const std::string Simple(const std::string &title, [[maybe_unused]] std::size_t title_lenpadd, const ValueLable &time) {
            return FORMAT(simpleFomrat, title, time);
        }

        /**
         * @brief A more elaborate print function for Timer class.
         */
        static const std::string Big(const std::string &title, std::size_t title_lenpadd, const ValueLable &time) {
            std::string times = FORMAT(bigTimesFotmat, time);
            const auto times_len = times.length() + 3;
            const auto tot_len = title_lenpadd + times_len;
            const auto title_time_section = FORMAT(bigTitleTimeFotmat, title, title_lenpadd - 4, times, times_len + 1);
            return FORMAT(bigFotmat, "", tot_len, title_time_section);
        }

        /**
         * @brief A compact print function for Timer class.
         */
        static const std::string Compact(const std::string &title, [[maybe_unused]] std::size_t title_lenpadd, const ValueLable &time) {
            return FORMAT(compatFotmat, title, time);
        }

        /**
         * @brief A detailed print function for Timer class.
         */
        static const std::string Detailed(const std::string &title, [[maybe_unused]] std::size_t title_lenpadd, const ValueLable &time) {
            return FORMAT(detaildFotmat, title, time);
        }

        static const std::string createPatterm(std::size_t title_lenpadd) {
            const auto ntlenpadd = title_lenpadd / 4;
            return FORMAT(blockPatternFotmat, "*", ntlenpadd);
        }
        /**
         * @brief A block style print function for Timer class.
         */
        static const std::string Block(const std::string &title, std::size_t title_lenpadd, const ValueLable &time) {
            const auto patternf = createPatterm(title_lenpadd);
            const auto times = FORMAT(blockTimesFotmat, time);
            return FORMAT(blockFotmat, patternf, title_lenpadd, title, times);
        }
        /**
         * @brief A minimal print function for Timer class.
         */
        static const std::string Minimal(const std::string &title, const std::string &time) { return FORMAT(minimalFotmat, title, time); }

        /**
         * @brief Standard constructor for Timer class.
         *  Standard constructor, can set title and print function
         */
        explicit Timer(const std::string &title = "Timer", const time_print_t &time_print = Simple)
          : title_(title), title_lenpadd(title.length() + 10), time_print_(time_print), start_(clock::now()) {}

        Timer(const Timer &other) = delete;              /// Delete copy constructor
        Timer &operator=(const Timer &other) = delete;   /// Delete copy assignment operator
        Timer(const Timer &&other) = delete;             /// Delete move constructor
        Timer &operator=(const Timer &&other) = delete;  /// Delete move assignment operator

        /**
         * @brief Time a function by running it multiple times.
         * @param f The function to be timed.
         * @param target_time Target time in seconds.
         * @return A string with timing information.
         */
        // NOLINTNEXTLINE(*-identifier-length)
        [[nodiscard]] std::string time_it(const std::function<void()> &f, long double target_time = 1) {
            const time_point start = start_;
            // NOLINTNEXTLINE(clang-analyzer-cplusplus.InnerPointer)
            auto total_time = C_LD(NAN);

            start_ = clock::now();
            std::size_t n = 0;  // NOLINT(*-identifier-length)
            do {                // NOLINT(*-avoid-do-while)
                f();
                nanolld elapsed = clock::now() - start_;
                total_time = elapsed.count();
            } while(n++ < MFACTOR && total_time < target_time);
            const auto total_timef = C_LD(total_time / C_LD(n));
            std::string out = FORMAT(timeItFotmat, make_time_str(total_timef), std::to_string(n));
            start_ = start;
            return out;
        }

        /**
         * @brief Get the elapsed time in seconds.
         * @return Elapsed time in seconds.
         */
        [[nodiscard]] inline long double make_time() const noexcept {
            const nanolld elapsed = clock::now() - start_;
            return elapsed.count();
        }
        /**
         * @brief Get the named times (seconds, milliseconds, microseconds, nanoseconds).
         * @param time The time in nanoseconds.
         * @return A tuple containing named times.
         */
        [[nodiscard]] static Times make_named_times(const long double time) noexcept { return Times{time}; }

        [[maybe_unused]] [[nodiscard]] Times multi_time() const noexcept { return Times{make_time()}; }

        /**
         * @brief Format the numerical value for the time string.
         * This formats the numerical value for the time string
         * @return A formatted time string.
         */
        [[nodiscard]] inline ValueLable make_time_str() const noexcept {  // NOLINT(modernize-use-nodiscard)
            const auto time = C_LD(make_time() / C_LD(cycles));
            return make_time_str(time);
        }
        //   LCOV_EXCL_START
        /**
         * @brief Format a given time value into a string.
         * This prints out a time string from a time
         * @param time The time value in nanoseconds.
         * @return A formatted time string.
         */
        [[nodiscard]] static inline ValueLable make_time_str(const long double time) noexcept {  // NOLINT(modernize-use-nodiscard)
            return make_named_times(time).getRelevantTimeframe();
        }
        // LCOV_EXCL_STOP

        /**
         * @brief Get a string representation of the Timer.
         * @return A string representation of the Timer.
         */
        [[nodiscard]] inline std::string to_string() const noexcept {
            return std::invoke(time_print_, title_, title_lenpadd, make_time_str());
        }

        /**
         * @brief Set the number of cycles to divide by.
         * Division sets the number of cycles to divide by (no graphical change)
         * @param val Number of cycles.
         * @return A reference to the Timer object.
         */
        Timer &operator/(std::size_t val) noexcept {
            cycles = val;
            return *this;
        }
    };

    /**
     * @brief Automatic Timer class that prints out the time upon destruction.
     */
    class AutoTimer : public Timer {
    public:
        using Timer::Timer;

        // Delete copy and move operations in AutoTimer
        AutoTimer(const AutoTimer &other) = delete;
        AutoTimer &operator=(const AutoTimer &other) = delete;
        AutoTimer(AutoTimer &&other) = delete;
        AutoTimer &operator=(AutoTimer &&other) = delete;

        /**
         * @brief Destructor for AutoTimer class that prints the time string.
         */
        ~AutoTimer() { LINFO(to_string()); }
    };
}  // namespace vnd

/** \cond */
/**
 * @brief Specialization of the fmt::formatter for the Timer class.
 */
template <> struct fmt::formatter<vnd::Timer> : formatter<std::string_view> {  // NOLINT(*-include-cleaner)
                                                                               /**
                                                                                * @brief Format the Timer object into a string view.
                                                                                * @param timer The Timer object.
                                                                                * @param ctx The format context.
                                                                                * @return A formatted string view.
                                                                                */
    auto format(const vnd::Timer &timer, format_context &ctx) const -> format_context::iterator {
        return formatter<std::string_view>::format(timer.to_string(), ctx);
    }
};

template <> struct std::formatter<vnd::Timer, char> {
    template <class ParseContext> constexpr auto parse(ParseContext &&ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

    template <typename FormatContext> auto format(const vnd::Timer &timer, FormatContext &ctx) const -> decltype(ctx.out()) {
        return std::format_to(ctx.out(), "{}", timer.to_string());
    }
};

/** \endcond */

/// This prints out the time if shifted into a std::cout like stream.
inline std::ostream &operator<<(std::ostream &in, const vnd::Timer &timer) { return in << timer.to_string(); }

// NOLINTEND(*-include-cleaner)
DISABLE_WARNINGS_POP()
#if defined(__GNUC__) && (__GNUC__ >= 11)
#pragma GCC diagnostic pop
#endif
