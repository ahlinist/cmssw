package cms.dqm.workflow;

import oracle.jdbc.pool.OracleDataSource;
import java.sql.*; 
import java.util.*; 
import javax.naming.*;

public class DBWorker {

  private Connection conn;

  private static boolean last_connect_result = false;

  public DBWorker() throws Exception {
    this(false);
  }

  public DBWorker(boolean readOnly) throws Exception {
    Context initContext = new InitialContext();
    Context envContext = (Context) initContext.lookup("java:/comp/env");
    if (envContext == null) throw new NamingException("Error: No Context");
    OracleDataSource ds = (OracleDataSource) envContext.lookup(WebUtils.GetEnv("db_name"));
    ds.setConnectionCachingEnabled(true);

    if (ds == null) throw new Exception("Error: No DataSource");

    String username = WebUtils.GetEnv("db_username");
    if (username == null) throw new Exception("Error: No DB username");

    if (ds.getUser() == null || !ds.getUser().equals(username) || !last_connect_result) {
      ds.setUser(username);
      String password = WebUtils.getPassword(username);
      if (password == null) throw new Exception("Error: DB user credential not found");
      ds.setPassword(password);
    }

    connect(ds, readOnly);
    if (!conn.isClosed()) last_connect_result = true;
  }

  public DBWorker(String jdbc, String username, String auth_file) throws Exception {
    this(jdbc, username, auth_file, false);
  }

  public DBWorker(String jdbc, String username, String auth_file, boolean readOnly) throws Exception {
    OracleDataSource ds = new OracleDataSource();
    ds.setConnectionCachingEnabled(true);
    ds.setURL(jdbc);
    if (ds.getUser() == null || !ds.getUser().equals(username)) {
      ds.setUser(username);
      String password = WebUtils.getPassword(username, auth_file);
      if (password == null) throw new Exception("Error: DB user credential not found");
      ds.setPassword(password);
    }
    connect(ds, readOnly);
    if (!conn.isClosed()) last_connect_result = true;
  }

  private void connect(OracleDataSource ds, boolean readOnly) throws Exception {
    conn = ds.getConnection();
    if (conn == null) throw new SQLException("Error: No Connection");
    conn.setReadOnly(readOnly);
  }

  public void setAutoCommit(boolean value) throws SQLException {
    conn.setAutoCommit(value);
  }

  public void commit() throws SQLException {
    conn.commit();
  }

  public void rollback() throws SQLException {
    conn.rollback();
  }

  public ResultSet executeSQL(String sql) throws SQLException {
    Statement stmt = conn.createStatement();
    return stmt.executeQuery(sql);
  }

  public PreparedStatement prepareSQL(String sql) throws SQLException { 
    PreparedStatement pstmt = conn.prepareStatement(sql);
    return pstmt;
  }

  public boolean executeSQLSilent(String sql, int parameter) throws SQLException {
    PreparedStatement pstmt = prepareSQL(sql);
    pstmt.setInt(1, parameter);
    return pstmt.execute();
  }

  public int getFirstRowValueInt(String sql, int parameter, int fieldnr) throws SQLException {
    PreparedStatement pstmt = prepareSQL(sql);
    pstmt.setInt(1, parameter);
    ResultSet res = pstmt.executeQuery();
    res.next(); 
    int i = res.getInt(fieldnr);
    res.close();
    return i;
  }

  public int getFirstRowValueInt(PreparedStatement pstmt) throws SQLException {
    ResultSet res = pstmt.executeQuery();
    res.next();
    int i = res.getInt(1);
    res.close();
    return i;
  }

  public String getFirstRowValueString(String sql, int parameter, int fieldnr) throws SQLException {
    PreparedStatement pstmt = prepareSQL(sql);
    pstmt.setInt(1, parameter);
    ResultSet res = pstmt.executeQuery();
    res.next();
    String s = res.getString(fieldnr);
    res.close();
    return s;
  }

  public Vector<String> getStringList(String sql, int fieldnr) throws SQLException {
    return getStringList(prepareSQL(sql), fieldnr);
  }

  public Vector<String> getStringList(PreparedStatement pstmt, int fieldnr) throws SQLException {
    Vector<String> list = new Vector<String>();
    ResultSet r = pstmt.executeQuery();
    while (r.next()) list.add(r.getString(fieldnr));
    r.close();
    return list;
  }

  public Hashtable<String, String> getStringsMap(PreparedStatement pstmt) throws SQLException {
    Hashtable<String,String> list = new Hashtable<String,String>();
    ResultSet r = pstmt.executeQuery();
    while (r.next()) {
      list.put(r.getString(1), r.getString(2));
    }
    r.close();
    return list;
  }

  public void close() {
    try {
      conn.close();
    } catch (Exception e) { }
  }

  protected void finalize() throws Throwable {
    close();
  }

  public static void setStringNull(PreparedStatement pstmt, String value, int number) throws SQLException {
    if (value == null) {
      pstmt.setNull(number, java.sql.Types.VARCHAR); 
    } else {
      pstmt.setString(number, value); 
    }
  }


}
