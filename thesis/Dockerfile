FROM debian:testing-slim

RUN apt-get -qq update && apt-get install -y \
  biber \
  wget \
  texlive-bibtex-extra \
  texlive-fonts-extra \
  texlive-fonts-recommended \
  texlive-formats-extra \
  texlive-lang-czechslovak \
  texlive-lang-english \
  texlive-latex-extra \
  texlive-latex-recommended \
  texlive-luatex \
  texlive-pictures \
  texlive-publishers \
  texlive-science \
  texlive \
  ghostscript \
  bzip2 \
  make

RUN  wget https://perso.ensta-paris.fr/~kielbasi/tikzuml/var/files/src/tikzuml-v1.0-2016-03-29.tbz -O /tmp/tikzuml.tbz && \
     mkdir -p /usr/share/texlive/texmf-dist/tex/latex/tikz-uml && \
     tar -xjf /tmp/tikzuml.tbz -C /usr/share/texlive/texmf-dist/tex/latex/tikz-uml --strip-components=1 && \
     texhash && rm /tmp/tikzuml.tbz

RUN rm -fr /var/lib/apt /var/cache/apt

WORKDIR /data
COPY . .

