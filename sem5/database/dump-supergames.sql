--
-- PostgreSQL database dump
--

-- Dumped from database version 10.21 (Debian 10.21-1.pgdg90+1)
-- Dumped by pg_dump version 10.21 (Debian 10.21-1.pgdg90+1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: internal; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA internal;


ALTER SCHEMA internal OWNER TO postgres;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: cart_elements; Type: TABLE; Schema: internal; Owner: postgres
--

CREATE TABLE internal.cart_elements (
    id integer NOT NULL,
    game integer NOT NULL,
    "user" integer NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE internal.cart_elements OWNER TO postgres;

--
-- Name: cart_elements_id_seq; Type: SEQUENCE; Schema: internal; Owner: postgres
--

CREATE SEQUENCE internal.cart_elements_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE internal.cart_elements_id_seq OWNER TO postgres;

--
-- Name: cart_elements_id_seq; Type: SEQUENCE OWNED BY; Schema: internal; Owner: postgres
--

ALTER SEQUENCE internal.cart_elements_id_seq OWNED BY internal.cart_elements.id;


--
-- Name: collection_elements; Type: TABLE; Schema: internal; Owner: postgres
--

CREATE TABLE internal.collection_elements (
    id integer NOT NULL,
    game integer NOT NULL,
    "user" integer NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE internal.collection_elements OWNER TO postgres;

--
-- Name: collection_elements_id_seq; Type: SEQUENCE; Schema: internal; Owner: postgres
--

CREATE SEQUENCE internal.collection_elements_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE internal.collection_elements_id_seq OWNER TO postgres;

--
-- Name: collection_elements_id_seq; Type: SEQUENCE OWNED BY; Schema: internal; Owner: postgres
--

ALTER SEQUENCE internal.collection_elements_id_seq OWNED BY internal.collection_elements.id;


--
-- Name: friendship; Type: TABLE; Schema: internal; Owner: postgres
--

CREATE TABLE internal.friendship (
    id integer NOT NULL,
    source integer NOT NULL,
    target integer NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE internal.friendship OWNER TO postgres;

--
-- Name: friendship_id_seq; Type: SEQUENCE; Schema: internal; Owner: postgres
--

CREATE SEQUENCE internal.friendship_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE internal.friendship_id_seq OWNER TO postgres;

--
-- Name: friendship_id_seq; Type: SEQUENCE OWNED BY; Schema: internal; Owner: postgres
--

ALTER SEQUENCE internal.friendship_id_seq OWNED BY internal.friendship.id;


--
-- Name: developers; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.developers (
    id integer NOT NULL,
    email text NOT NULL,
    password character(32) NOT NULL,
    name text NOT NULL,
    description text DEFAULT ''::text NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.developers OWNER TO postgres;

--
-- Name: developers_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.developers_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.developers_id_seq OWNER TO postgres;

--
-- Name: developers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.developers_id_seq OWNED BY public.developers.id;


--
-- Name: games; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.games (
    id integer NOT NULL,
    name text NOT NULL,
    description text DEFAULT ''::text NOT NULL,
    developer integer NOT NULL,
    price double precision DEFAULT 0.0 NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.games OWNER TO postgres;

--
-- Name: games_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.games_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.games_id_seq OWNER TO postgres;

--
-- Name: games_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.games_id_seq OWNED BY public.games.id;


--
-- Name: reviews; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.reviews (
    id integer NOT NULL,
    game integer NOT NULL,
    "user" integer NOT NULL,
    rating smallint NOT NULL,
    content text DEFAULT ''::text NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.reviews OWNER TO postgres;

--
-- Name: reviews_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.reviews_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.reviews_id_seq OWNER TO postgres;

--
-- Name: reviews_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.reviews_id_seq OWNED BY public.reviews.id;


--
-- Name: users; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.users (
    id integer NOT NULL,
    login text NOT NULL,
    password character(32) NOT NULL,
    name text NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.users_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.users_id_seq OWNER TO postgres;

--
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- Name: cart_elements id; Type: DEFAULT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements ALTER COLUMN id SET DEFAULT nextval('internal.cart_elements_id_seq'::regclass);


--
-- Name: collection_elements id; Type: DEFAULT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements ALTER COLUMN id SET DEFAULT nextval('internal.collection_elements_id_seq'::regclass);


--
-- Name: friendship id; Type: DEFAULT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship ALTER COLUMN id SET DEFAULT nextval('internal.friendship_id_seq'::regclass);


--
-- Name: developers id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.developers ALTER COLUMN id SET DEFAULT nextval('public.developers_id_seq'::regclass);


--
-- Name: games id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games ALTER COLUMN id SET DEFAULT nextval('public.games_id_seq'::regclass);


--
-- Name: reviews id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews ALTER COLUMN id SET DEFAULT nextval('public.reviews_id_seq'::regclass);


--
-- Name: users id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- Data for Name: cart_elements; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.cart_elements (id, game, "user", date) FROM stdin;
\.


--
-- Data for Name: collection_elements; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.collection_elements (id, game, "user", date) FROM stdin;
5	2	5	2022-11-14 18:04:25.402792
6	4	5	2022-11-14 18:04:27.738747
7	1	5	2022-11-14 19:02:09.332232
8	2	7	2022-11-14 19:18:00.824322
9	5	7	2022-11-14 19:18:05.721139
10	4	7	2022-11-15 07:47:03.679684
11	6	7	2022-11-15 07:51:27.356674
12	6	8	2022-11-15 09:05:37.249763
\.


--
-- Data for Name: friendship; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.friendship (id, source, target, date) FROM stdin;
21	8	6	2022-11-15 09:06:09.028303
\.


--
-- Data for Name: developers; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.developers (id, email, password, name, description, date) FROM stdin;
1	debils.tech@gmail.com	25d55ad283aa400af464c76d713c07ad	Debils Technologies		2022-11-06 15:47:20.729339
2	support@chucklefish.org	25d55ad283aa400af464c76d713c07ad	Chucklefish	Маленькая инди компания	2022-11-07 13:37:31.10793
3	mail@idsoftware.com	b4f945433ea4c369c12741f62a23ccc0	id Software		2022-11-14 19:07:51.617211
\.


--
-- Data for Name: games; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.games (id, name, description, developer, price, date) FROM stdin;
1	Super Flexer 2D	Абсолютно крутая игра!	1	0	2022-11-07 00:02:41.581935
4	Animal Crossing: New Horizons	Новая часть игры про разведение животных на острове! Здесь есть лиса, медведь, собака, кошка (больше не знаю)...\nИгра выполнена в "мультяшном" стиле\n\n(Исключительно для платформы Nintendo Switch)	1	1500	2022-11-07 13:17:18.994499
2	Starbound	Прикольная игра про космос	2	399	2022-11-07 01:45:16.143021
5	Doom	Убивай демонов, стреляй из оружия, очисти мир от служителей ада!	3	600	2022-11-14 22:05:58.972199
6	Doom Eternal	Продолжение легендарной игры про демонов. Теперь можно мочить демонов в разы быстрее с новым огнемётом и мечом прямо в броне!\\nДля любителей паркура добавлена возможность карабкаться по стенам и цепляться крюком за объекты	3	2000	2022-11-14 22:09:48.802467
\.


--
-- Data for Name: reviews; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.reviews (id, game, "user", rating, content, date) FROM stdin;
1	5	6	10	Просто обожаю убивать демонов! 10/10!!!	2022-11-14 23:51:46.774
2	1	5	9	Почти идеальная игра. 1 балл снял из-за того, что глючит на моём телефоне	2022-11-15 09:17:42.654193
3	1	8	10	Лучшая игра в истории человечества!!!	2022-11-15 09:17:42.670324
\.


--
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (id, login, password, name, date) FROM stdin;
5	uslashvlad	81dc9bdb52d04dc20036dbd8313ed055	u/vlad	2022-11-06 15:45:30.507127
7	test1	81dc9bdb52d04dc20036dbd8313ed055	Тестовый пользователь 1	2022-11-14 19:03:57.5851
8	test2	d93591bdf7860e1e4ee2fca799911215	Тестовый пользователь 2	2022-11-14 19:04:17.046483
6	a1234	b59c67bf196a4758191e42f76670ceba	Cool guy	2022-11-06 20:57:14.023344
\.


--
-- Name: cart_elements_id_seq; Type: SEQUENCE SET; Schema: internal; Owner: postgres
--

SELECT pg_catalog.setval('internal.cart_elements_id_seq', 37, true);


--
-- Name: collection_elements_id_seq; Type: SEQUENCE SET; Schema: internal; Owner: postgres
--

SELECT pg_catalog.setval('internal.collection_elements_id_seq', 12, true);


--
-- Name: friendship_id_seq; Type: SEQUENCE SET; Schema: internal; Owner: postgres
--

SELECT pg_catalog.setval('internal.friendship_id_seq', 21, true);


--
-- Name: developers_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.developers_id_seq', 3, true);


--
-- Name: games_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.games_id_seq', 6, true);


--
-- Name: reviews_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.reviews_id_seq', 3, true);


--
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.users_id_seq', 8, true);


--
-- Name: cart_elements cart_elements_pk; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT cart_elements_pk PRIMARY KEY (id);


--
-- Name: collection_elements collection_elements_pk; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements
    ADD CONSTRAINT collection_elements_pk PRIMARY KEY (id);


--
-- Name: friendship friendship_pk; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship
    ADD CONSTRAINT friendship_pk PRIMARY KEY (id);


--
-- Name: developers developers_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.developers
    ADD CONSTRAINT developers_pk PRIMARY KEY (id);


--
-- Name: developers email_un; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.developers
    ADD CONSTRAINT email_un UNIQUE (email);


--
-- Name: games games_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games
    ADD CONSTRAINT games_pk PRIMARY KEY (id);


--
-- Name: users login_un; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT login_un UNIQUE (login);


--
-- Name: reviews reviews_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews
    ADD CONSTRAINT reviews_pk PRIMARY KEY (id);


--
-- Name: users users_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pk PRIMARY KEY (id);


--
-- Name: cart_elements created_by_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT created_by_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: collection_elements created_by_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements
    ADD CONSTRAINT created_by_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: cart_elements created_on_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT created_on_fk FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: collection_elements created_on_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements
    ADD CONSTRAINT created_on_fk FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: friendship source_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship
    ADD CONSTRAINT source_fk FOREIGN KEY (source) REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: friendship target_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship
    ADD CONSTRAINT target_fk FOREIGN KEY (target) REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: games created_by_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games
    ADD CONSTRAINT created_by_fk FOREIGN KEY (developer) REFERENCES public.developers(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: reviews created_by_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews
    ADD CONSTRAINT created_by_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- Name: reviews created_on_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews
    ADD CONSTRAINT created_on_fk FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

