#ifndef _GNUPLOT_WRAPPER
#define _GNUPLOT_WRAPPER

#include <string>

#include <cstdio>
#include <cstddef>

#define RANGE_X 1
#define RANGE_Y 2
#define RANGE_XY (RANGE_X | RANGE_Y)

#define IS_PLOT(flag) ((flag) > 0)
#define IS_SPLOT(flag) ((flag)) < 0)

namespace plt {

	class axis_range {
	private:
		double start, end;
		friend class gnuplot;

		std::string get_plot_header();

	public:
		axis_range(double start, double end);
		axis_range();

		double get_start() const {
			return start;
		}

		double get_end() const {
			return end;
		}
	};

	class terminal_output {
	private:
		size_t sz_x, sz_y;
		std::string fname;
		std::string fmt;
		friend class gnuplot;
		
	public:
		terminal_output(const std::string& fname,
						const std::string& fmt,
						size_t sz_x,
						size_t sz_y
						) {
			this->fname = fname;
			this->fmt = fmt;
			this->sz_x = sz_x;
			this->sz_y = sz_y;
		}

		terminal_output() {
			this->fname = "plot.png";
			this->fmt = "png";
			this->sz_x = 600lu;
			this->sz_y = 400lu;
		}
	};

	class plot_meta {
	private:
		std::string title;
		bool title_is_set;
		friend class gnuplot;
		
	public:
		plot_meta() {
			this->title_is_set = false;
		}

		plot_meta(std::string title) {
			this->title_is_set = true;
			this->title = title;
		}

		bool has_title() const {
			return title_is_set;
		}

		std::string get_title() const {
			return title;
		}
	};
	
	class gnuplot {
	private:
		FILE *p;
		FILE *macro_out;

		char range_flag;
		axis_range xr, yr;

		char plot_flag;
		std::string plot_buf;

		plt::terminal_output term_out;
		std::string get_plot_header();

		void init();

	public:
		gnuplot();
		gnuplot(const std::string& fname, const std::string& fmt, size_t sz_x, size_t sz_y);
		
		~gnuplot();
		
		void send_raw(const std::string& s);
		void flush();
		void repaint();
		void clear();
		
		void load_file(const std::string& fname);

		void set_output(const std::string& fname, const std::string& fmt, size_t sz_x, size_t sz_y);
		void set_output(const plt::terminal_output& term_out);
		void set_macro_output(FILE *macro_out);

		void set_xrange(const plt::axis_range& r);
		void set_yrange(const plt::axis_range& r);
		void set_xrange(double xstart, double xend);
		void set_yrange(double ystart, double yend);
		void unset_xrange();
		void unset_yrange();

		void plot_function(const std::string& func);
		void plot_data(const std::string& fname);
		
		void plot_function(const std::string& func, const plot_meta& meta);
		void plot_data(const std::string& fname, const plot_meta& meta);
		
		void set_title(const std::string& s);
		void set_xlabel(const std::string& s);
		void set_ylabel(const std::string& s);

		void set_logscale(const std::string& axes, double base);
		void set_logscale(const std::string& axes);
		void unset_logscale(const std::string& axes);

		void save_settings(const std::string& fname);
		void save_macro(const std::string& fname);
	};

}
#endif
