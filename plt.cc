#include <cstdlib>
#include <iostream>
#include "plt.hh"

// Gnu Plot class
plt::gnuplot::gnuplot() {
	init();
}

plt::gnuplot::gnuplot(const std::string& fname,
						const std::string& fmt,
						size_t sz_x,
						size_t sz_y) {
	
	init();
	set_output(fname, fmt, sz_x, sz_y);
}

plt::gnuplot plt::gnuplot::operator=(const plt::gnuplot& plot) {
	return plt::gnuplot(plot); // copy c-tor
}

plt::gnuplot::gnuplot(const plt::gnuplot& plot) {
	init();
	this->plot_buf = plot.plot_buf;
}

void plt::gnuplot::init() {
	this->p = popen("gnuplot", "w");
   	this->range_flag = 0;
	this->plot_flag = 0;
	this->plot_buf = "";
	this->macro_out = nullptr;
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

void plt::gnuplot::repaint() {
	set_output(term_out);
	send_raw(get_plot_header() + plot_buf);
}

void plt::gnuplot::clear() {
	send_raw("clear");
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

void plt::gnuplot::plot_function(const std::string& f, const plot_meta& meta) {
	plot_buf += (plot_buf == "" ? f : ", " + f)
		+ (meta.has_title() ? + " title \"" + meta.get_title() + "\"" : "");
	repaint();
}

void plt::gnuplot::plot_data(const std::string& fname, const plot_meta& meta) {
	plot_buf += (plot_buf == "" ? "\"" + fname + "\"" : ", \"" + fname + "\"")
		+ (meta.has_title() ? " title \"" + meta.get_title() + "\"" : "");
	repaint();
}

void plt::gnuplot::plot_function(const std::string& func) {
	plt::plot_meta empty_meta;
	plot_function(func, empty_meta);
}

void plt::gnuplot::plot_data(const std::string& fname) {
	plt::plot_meta empty_meta;
	plot_data(fname, empty_meta);
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

void plt::gnuplot::set_output(const plt::terminal_output& term_out) {
	this->term_out = term_out;

	fprintf(p, "set terminal %s size %lu,%lu; set output \"%s\"\n",
			term_out.fmt.c_str(),
			term_out.sz_x,
			term_out.sz_y,
			term_out.fname.c_str()
		   );

	if (macro_out) {
		fprintf(macro_out, "set terminal %s size %lu,%lu; set output \"%s\"\n",
			term_out.fmt.c_str(),
			term_out.sz_x,
			term_out.sz_y,
			term_out.fname.c_str()
		   );
	}
}

void plt::gnuplot::set_output(const std::string& fname, const std::string& fmt, size_t sz_x, size_t sz_y) {
	plt::terminal_output term_out(fname, fmt, sz_x, sz_y);
	set_output(term_out);
}

void plt::gnuplot::set_macro_output(FILE *macro_out) {
	this->macro_out = macro_out;
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

void plt::gnuplot::save_settings(const std::string& fname) {
	send_raw("save set \"" + fname + "\"");
}

void plt::gnuplot::save_macro(const std::string& fname) {
	send_raw("save \"" + fname + "\"");
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

