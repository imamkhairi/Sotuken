
\NeedsTeXFormat{pLaTeX2e}
%\ProvidesClass{jpreprint}[2012/10/24 卒業研究発表会予稿]
\ExecuteOptions{12pt,a4paper}
\ProcessOptions\relax
\LoadClass[]{ujarticle}
%\RequirePackage[dvips]{graphics}
\RequirePackage{times}
\RequirePackage{multicol}

\def\jtitle#1{\def\@jtitle{#1}}
\def\jauthor#1{\def\@jauthor{#1}}
\def\jteacher#1{\def\@jteacher{#1}}
\def\jaffiliation#1{\def\@jaffiliation{#1}}

\newenvironment{tablehere}
  {\def\@captype{table}}
  {}
\newenvironment{figurehere}
  {\def\@captype{figure}}
  {}

\textwidth 180mm
\textheight 207mm
\oddsidemargin -10mm
\evensidemargin -10mm
\columnsep 10mm
\columnseprule 0pt
\textheight=\paperheight
\advance\textheight -45mm
\headheight=0pt
\topmargin=-1in
\advance\topmargin 12mm

\def\maketitle{%
  \vbox {%
    \hbox to \textwidth{\vtop{\hsize=\textwidth\centering{\bfseries\fontsize{24pt}{26pt}\selectfont\@jtitle}}}
    \vskip 8pt
    \hbox to \textwidth{\hss\@jauthor (\@jaffiliation)\hss}
    \hbox to \textwidth{\hss 指導教員~~~~\@jteacher\hss}
  }
}

\pagestyle{empty}
