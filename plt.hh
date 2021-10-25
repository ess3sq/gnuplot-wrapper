#ifndef _GNUPLOT_WRAPPER
#define _GNUPLOT_WRAPPER

#include <string>

#include <cstdio>
#include <cstddef>

#define RANGE_X 1
#define RANGE_Y 2
#define RANGE_XY (RANGE_X | RANGE_Y)

namespace plt {

	class axis_range {
	private:
		double start, end;
		friend class gnuplot;

		std::string get_plot_header();

	public:
		axis_range(double start, double end);
		axis_range();

		double get_start() {
			return start;
		}

		double get_end() {
			return end;
		}
	};
	
	class gnuplot {
	private:
		FILE *p;
		FILE *macro_out;
		char range_flag;
		axis_range xr, yr;

		std::string get_plot_header();
		
	public:
		gnuplot();
		gnuplot(FILE *macro_out);
		void init(FILE *macro_out);

		~gnuplot();
		
		void send_raw(const std::string& s);
		void flush();

		void load_file(const std::string& fname);

		void set_output(const std::string& fname, const std::string& fmt, size_t sz_x, size_t sz_y);

		void set_xrange(const plt::axis_range& r);
		void set_yrange(const plt::axis_range& r);
		void set_xrange(double xstart, double xend);
		void set_yrange(double ystart, double yend);
		void unset_xrange();
		void unset_yrange();

		void plot_function(const std::string& func);
		void plot_data(const std::string& fname);
		
		void set_title(const std::string& s);
		void set_xlabel(const std::string& s);
		void set_ylabel(const std::string& s);

		void set_logscale(const std::string& axes, double base);
		void set_logscale(const std::string& axes);
		void unset_logscale(const std::string& axes);
	};

}
#endif
