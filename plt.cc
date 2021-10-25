#include <iostream>
#include "plt.hh"

// Gnu Plot class
plt::gnuplot::gnuplot() {
	init(nullptr);
}

plt::gnuplot::gnuplot(FILE *macro_out) {
	init(macro_out);
}

void plt::gnuplot::init(FILE *macro_out) {
	this->p = popen("gnuplot", "w");
   	this->range_flag = 0;
	this->macro_out = macro_out;

}

plt::gnuplot::~gnuplot() {
	send_raw("exit");
	pclose(p);
}

void plt::gnuplot::send_raw(const std::string& s) {
	fprintf(this->p, "%s\n", s.c_str());
	if (macro_out)
		fprintf(this->macro_out, "%s\n", s.c_str());
}

void plt::gnuplot::load_file(const std::string& fname) {
	send_raw("load \"" + fname + "\"");
}

void plt::gnuplot::flush() {
	fflush(p);
	if (macro_out) fflush(macro_out);
}

std::string plt::gnuplot::get_plot_header() {
	return "plot "
			 + (RANGE_X & range_flag ? "["
				+ std::to_string(xr.start)
				+ ":"
				+ std::to_string(xr.end)
				+ "] "
				: "[] "
			   )
			 + (RANGE_Y & range_flag ? "["
				+ std::to_string(yr.start)
				+ ":"
				+ std::to_string(yr.end)
				+ "] "
				: "[] "
				)
		;
}

void plt::gnuplot::plot_function(const std::string& f) {
	send_raw(get_plot_header() + f);
}

void plt::gnuplot::plot_data(const std::string& fname) {
	send_raw(get_plot_header() + + "\"" + fname + "\"");
}

void plt::gnuplot::set_title(const std::string& s) {
	send_raw("set title \"" + s + "\"");
}

void plt::gnuplot::set_xlabel(const std::string& s) {
	send_raw("set xlabel \"" + s + "\"");
}

void plt::gnuplot::set_ylabel(const std::string& s) {
	send_raw("set ylabel \"" + s + "\"");
}

void plt::gnuplot::set_output(const std::string& fname, const std::string& fmt, size_t sz_x, size_t sz_y) {
	fprintf(p, "set terminal %s size %lu,%lu; set output \"%s\"\n",
			fmt.c_str(), sz_x, sz_y, fname.c_str()
		   );
	
	if (macro_out) {
		fprintf(macro_out, "set terminal %s size %lu,%lu; set output \"%s\"\n",
			fmt.c_str(), sz_x, sz_y, fname.c_str()
			);
	}
}

void plt::gnuplot::set_xrange(const plt::axis_range& r) {
	xr = r;
	range_flag |= RANGE_X;
}

void plt::gnuplot::set_yrange(const plt::axis_range& r) {
	yr = r;
	range_flag |= RANGE_Y;
}

void plt::gnuplot::set_xrange(double xstart, double xend) {
	plt::axis_range xr(xstart, xend);
	set_xrange(xr);
}

void plt::gnuplot::set_yrange(double ystart, double yend) {
	plt::axis_range yr(ystart, yend);
	set_yrange(yr);
}


void plt::gnuplot::unset_xrange() {
	range_flag &= ~RANGE_X;
}

void plt::gnuplot::unset_yrange() {
	range_flag &= ~RANGE_Y;
}

void plt::gnuplot::set_logscale(const std::string& axes, double base) {
	fprintf(p, "set logscale %s %lf\n", axes.c_str(),  base);
	if (macro_out)
		fprintf(macro_out, "set logscale %s %lf\n", axes.c_str(),  base);
}

void plt::gnuplot::set_logscale(const std::string& axes) {
	set_logscale(axes, 10.);
}

void plt::gnuplot::unset_logscale(const std::string& axes) {
	send_raw("unset logscale " + axes);	
}


// Plot Range class

plt::axis_range::axis_range(double start, double end) {
	this->start = start;
	this->end = end;
}

plt::axis_range::axis_range() {
	this->start = 0;
	this->end = 0;
}

