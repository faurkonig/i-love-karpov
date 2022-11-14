--
-- PostgreSQL database dump
--

-- Dumped from database version 10.21 (Debian 10.21-1.pgdg90+1)
-- Dumped by pg_dump version 14.5

-- Started on 2022-11-14 22:24:08 MSK

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
-- TOC entry 3 (class 2615 OID 16513)
-- Name: internal; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA internal;


ALTER SCHEMA internal OWNER TO postgres;

--
-- TOC entry 7 (class 2615 OID 16514)
-- Name: public; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA public;


ALTER SCHEMA public OWNER TO postgres;

--
-- TOC entry 2951 (class 0 OID 0)
-- Dependencies: 7
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA public IS 'standard public schema';


SET default_tablespace = '';

--
-- TOC entry 208 (class 1259 OID 16734)
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
-- TOC entry 207 (class 1259 OID 16732)
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
-- TOC entry 2952 (class 0 OID 0)
-- Dependencies: 207
-- Name: cart_elements_id_seq; Type: SEQUENCE OWNED BY; Schema: internal; Owner: postgres
--

ALTER SEQUENCE internal.cart_elements_id_seq OWNED BY internal.cart_elements.id;


--
-- TOC entry 210 (class 1259 OID 16753)
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
-- TOC entry 209 (class 1259 OID 16751)
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
-- TOC entry 2953 (class 0 OID 0)
-- Dependencies: 209
-- Name: collection_elements_id_seq; Type: SEQUENCE OWNED BY; Schema: internal; Owner: postgres
--

ALTER SEQUENCE internal.collection_elements_id_seq OWNED BY internal.collection_elements.id;


--
-- TOC entry 206 (class 1259 OID 16715)
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
-- TOC entry 205 (class 1259 OID 16713)
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
-- TOC entry 2954 (class 0 OID 0)
-- Dependencies: 205
-- Name: friendship_id_seq; Type: SEQUENCE OWNED BY; Schema: internal; Owner: postgres
--

ALTER SEQUENCE internal.friendship_id_seq OWNED BY internal.friendship.id;


--
-- TOC entry 198 (class 1259 OID 16648)
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
-- TOC entry 197 (class 1259 OID 16646)
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
-- TOC entry 2955 (class 0 OID 0)
-- Dependencies: 197
-- Name: developers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.developers_id_seq OWNED BY public.developers.id;


--
-- TOC entry 200 (class 1259 OID 16661)
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
-- TOC entry 199 (class 1259 OID 16659)
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
-- TOC entry 2956 (class 0 OID 0)
-- Dependencies: 199
-- Name: games_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.games_id_seq OWNED BY public.games.id;


--
-- TOC entry 204 (class 1259 OID 16692)
-- Name: reviews; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.reviews (
    id integer NOT NULL,
    game integer NOT NULL,
    "user" integer NOT NULL,
    rating smallint NOT NULL,
    description text DEFAULT ''::text NOT NULL,
    date timestamp without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.reviews OWNER TO postgres;

--
-- TOC entry 203 (class 1259 OID 16690)
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
-- TOC entry 2957 (class 0 OID 0)
-- Dependencies: 203
-- Name: reviews_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.reviews_id_seq OWNED BY public.reviews.id;


--
-- TOC entry 202 (class 1259 OID 16680)
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
-- TOC entry 201 (class 1259 OID 16678)
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
-- TOC entry 2958 (class 0 OID 0)
-- Dependencies: 201
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- TOC entry 2780 (class 2604 OID 16737)
-- Name: cart_elements id; Type: DEFAULT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements ALTER COLUMN id SET DEFAULT nextval('internal.cart_elements_id_seq'::regclass);


--
-- TOC entry 2782 (class 2604 OID 16756)
-- Name: collection_elements id; Type: DEFAULT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements ALTER COLUMN id SET DEFAULT nextval('internal.collection_elements_id_seq'::regclass);


--
-- TOC entry 2778 (class 2604 OID 16718)
-- Name: friendship id; Type: DEFAULT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship ALTER COLUMN id SET DEFAULT nextval('internal.friendship_id_seq'::regclass);


--
-- TOC entry 2766 (class 2604 OID 16651)
-- Name: developers id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.developers ALTER COLUMN id SET DEFAULT nextval('public.developers_id_seq'::regclass);


--
-- TOC entry 2769 (class 2604 OID 16664)
-- Name: games id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games ALTER COLUMN id SET DEFAULT nextval('public.games_id_seq'::regclass);


--
-- TOC entry 2775 (class 2604 OID 16695)
-- Name: reviews id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews ALTER COLUMN id SET DEFAULT nextval('public.reviews_id_seq'::regclass);


--
-- TOC entry 2773 (class 2604 OID 16683)
-- Name: users id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- TOC entry 2943 (class 0 OID 16734)
-- Dependencies: 208
-- Data for Name: cart_elements; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.cart_elements (id, game, "user", date) FROM stdin;
31	6	7	2022-11-14 19:17:54.631738
\.


--
-- TOC entry 2945 (class 0 OID 16753)
-- Dependencies: 210
-- Data for Name: collection_elements; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.collection_elements (id, game, "user", date) FROM stdin;
5	2	5	2022-11-14 18:04:25.402792
6	4	5	2022-11-14 18:04:27.738747
7	1	5	2022-11-14 19:02:09.332232
8	2	7	2022-11-14 19:18:00.824322
9	5	7	2022-11-14 19:18:05.721139
\.


--
-- TOC entry 2941 (class 0 OID 16715)
-- Dependencies: 206
-- Data for Name: friendship; Type: TABLE DATA; Schema: internal; Owner: postgres
--

COPY internal.friendship (id, source, target, date) FROM stdin;
\.


--
-- TOC entry 2933 (class 0 OID 16648)
-- Dependencies: 198
-- Data for Name: developers; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.developers (id, email, password, name, description, date) FROM stdin;
1	debils.tech@gmail.com	25d55ad283aa400af464c76d713c07ad	Debils Technologies		2022-11-06 15:47:20.729339
2	support@chucklefish.org	25d55ad283aa400af464c76d713c07ad	Chucklefish	Маленькая инди компания	2022-11-07 13:37:31.10793
3	mail@idsoftware.com	b4f945433ea4c369c12741f62a23ccc0	id Software		2022-11-14 19:07:51.617211
\.


--
-- TOC entry 2935 (class 0 OID 16661)
-- Dependencies: 200
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
-- TOC entry 2939 (class 0 OID 16692)
-- Dependencies: 204
-- Data for Name: reviews; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.reviews (id, game, "user", rating, description, date) FROM stdin;
\.


--
-- TOC entry 2937 (class 0 OID 16680)
-- Dependencies: 202
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.users (id, login, password, name, date) FROM stdin;
5	uslashvlad	81dc9bdb52d04dc20036dbd8313ed055	u/vlad	2022-11-06 15:45:30.507127
7	test1	81dc9bdb52d04dc20036dbd8313ed055	Тестовый пользователь 1	2022-11-14 19:03:57.5851
8	test2	d93591bdf7860e1e4ee2fca799911215	Тестовый пользователь 2	2022-11-14 19:04:17.046483
6	a1234	b59c67bf196a4758191e42f76670ceba	Cool guy	2022-11-06 20:57:14.023344
\.


--
-- TOC entry 2959 (class 0 OID 0)
-- Dependencies: 207
-- Name: cart_elements_id_seq; Type: SEQUENCE SET; Schema: internal; Owner: postgres
--

SELECT pg_catalog.setval('internal.cart_elements_id_seq', 31, true);


--
-- TOC entry 2960 (class 0 OID 0)
-- Dependencies: 209
-- Name: collection_elements_id_seq; Type: SEQUENCE SET; Schema: internal; Owner: postgres
--

SELECT pg_catalog.setval('internal.collection_elements_id_seq', 9, true);


--
-- TOC entry 2961 (class 0 OID 0)
-- Dependencies: 205
-- Name: friendship_id_seq; Type: SEQUENCE SET; Schema: internal; Owner: postgres
--

SELECT pg_catalog.setval('internal.friendship_id_seq', 18, true);


--
-- TOC entry 2962 (class 0 OID 0)
-- Dependencies: 197
-- Name: developers_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.developers_id_seq', 3, true);


--
-- TOC entry 2963 (class 0 OID 0)
-- Dependencies: 199
-- Name: games_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.games_id_seq', 6, true);


--
-- TOC entry 2964 (class 0 OID 0)
-- Dependencies: 203
-- Name: reviews_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.reviews_id_seq', 1, false);


--
-- TOC entry 2965 (class 0 OID 0)
-- Dependencies: 201
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.users_id_seq', 8, true);


--
-- TOC entry 2799 (class 2606 OID 16740)
-- Name: cart_elements cart_elements_pk; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT cart_elements_pk PRIMARY KEY (id);


--
-- TOC entry 2801 (class 2606 OID 16759)
-- Name: collection_elements collection_elements_pk; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements
    ADD CONSTRAINT collection_elements_pk PRIMARY KEY (id);


--
-- TOC entry 2797 (class 2606 OID 16721)
-- Name: friendship friendship_pk; Type: CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship
    ADD CONSTRAINT friendship_pk PRIMARY KEY (id);


--
-- TOC entry 2785 (class 2606 OID 16658)
-- Name: developers developers_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.developers
    ADD CONSTRAINT developers_pk PRIMARY KEY (id);


--
-- TOC entry 2787 (class 2606 OID 16772)
-- Name: developers email_un; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.developers
    ADD CONSTRAINT email_un UNIQUE (email);


--
-- TOC entry 2789 (class 2606 OID 16672)
-- Name: games games_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games
    ADD CONSTRAINT games_pk PRIMARY KEY (id);


--
-- TOC entry 2791 (class 2606 OID 16774)
-- Name: users login_un; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT login_un UNIQUE (login);


--
-- TOC entry 2795 (class 2606 OID 16702)
-- Name: reviews reviews_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews
    ADD CONSTRAINT reviews_pk PRIMARY KEY (id);


--
-- TOC entry 2793 (class 2606 OID 16689)
-- Name: users users_pk; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pk PRIMARY KEY (id);


--
-- TOC entry 2808 (class 2606 OID 16746)
-- Name: cart_elements created_by_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT created_by_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2810 (class 2606 OID 16765)
-- Name: collection_elements created_by_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements
    ADD CONSTRAINT created_by_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2807 (class 2606 OID 16741)
-- Name: cart_elements created_on_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.cart_elements
    ADD CONSTRAINT created_on_fk FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2809 (class 2606 OID 16760)
-- Name: collection_elements created_on_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.collection_elements
    ADD CONSTRAINT created_on_fk FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2805 (class 2606 OID 16722)
-- Name: friendship source_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship
    ADD CONSTRAINT source_fk FOREIGN KEY (source) REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2806 (class 2606 OID 16727)
-- Name: friendship target_fk; Type: FK CONSTRAINT; Schema: internal; Owner: postgres
--

ALTER TABLE ONLY internal.friendship
    ADD CONSTRAINT target_fk FOREIGN KEY (target) REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2802 (class 2606 OID 16673)
-- Name: games created_by_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.games
    ADD CONSTRAINT created_by_fk FOREIGN KEY (developer) REFERENCES public.developers(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2804 (class 2606 OID 16708)
-- Name: reviews created_by_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews
    ADD CONSTRAINT created_by_fk FOREIGN KEY ("user") REFERENCES public.users(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2803 (class 2606 OID 16703)
-- Name: reviews created_on_fk; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.reviews
    ADD CONSTRAINT created_on_fk FOREIGN KEY (game) REFERENCES public.games(id) ON UPDATE CASCADE ON DELETE CASCADE;


-- Completed on 2022-11-14 22:24:08 MSK

--
-- PostgreSQL database dump complete
--

